#!/usr/bin/env bash
# Automate an IRC handshake via nc, then exit with SIGINT.
# Usage: ./irc_macro.sh [host] [port]
# Defaults: host=localhost, port=1234

set -Eeuo pipefail
trap '' PIPE

HOST="${1:-localhost}"
PORT="${2:-1234}"

PASS="hello123456"
NICK="hady"
USER_NAME="hawayda"
REALNAME="hady"
CHAN="#channel1"
TOPIC1="hafadssss"
KICK_NICK="hady"

# Make a unique FIFO and ensure cleanup
FIFO="$(mktemp -u /tmp/irc_fifo.XXXXXX)"
cleanup() {
  # Close writer FD if open
  { exec 3>&-; } 2>/dev/null || true
  # Kill nc if still running
  [[ -n "${NC_PID:-}" ]] && kill "$NC_PID" 2>/dev/null || true
  # Remove fifo
  [[ -p "$FIFO" ]] && rm -f "$FIFO" || true
}
trap cleanup EXIT

mkfifo "$FIFO"

# Start nc in the background; it reads from the FIFO and writes to your terminal.
# We add a short connect timeout (-w 3) if your nc supports it; harmless if not.
if nc -h 2>&1 | grep -q -- '-w '; then
  nc "$HOST" "$PORT" -w 3 <"$FIFO" &
else
  nc "$HOST" "$PORT" <"$FIFO" &
fi
NC_PID=$!

# Give nc a moment to connect; bail if it died (e.g., connection refused)
sleep 0.12
if ! kill -0 "$NC_PID" 2>/dev/null; then
  echo "[irc_macro] Unable to connect to ${HOST}:${PORT} (nc exited)" >&2
  exit 1
fi

# Keep the FIFO open for writing via FD 3 (prevents premature EOF to nc)
exec 3>"$FIFO"

send() { printf '%s\r\n' "$*" >&3; }

# === Scripted IRC dialogue ===
sleep 0.10; send "PASS ${PASS}"
sleep 0.10; send "NICK ${NICK}"
sleep 0.10; send "USER ${USER_NAME} 0 * :${REALNAME}"
sleep 0.10; send "JOIN ${CHAN}"
sleep 0.10; send "TOPIC ${CHAN} ${TOPIC1}"
sleep 0.10; send "TOPIC ${CHAN}"
sleep 0.10; send "KICK ${CHAN} ${KICK_NICK}"

# Let responses flush, then send Ctrl-C (SIGINT) to nc
sleep 0.30
kill -INT "$NC_PID" 2>/dev/null || true

# Close writer so nc can exit, then wait a beat
exec 3>&- 2>/dev/null || true
sleep 0.10
wait "$NC_PID" 2>/dev/null || true
