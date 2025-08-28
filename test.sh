#!/usr/bin/env bash
# irc_macro.sh — automate an IRC session, then exit by closing stdin + SIGINT.
# Usage: ./irc_macro.sh [host] [port]   (defaults: localhost 1234)

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

FIFO="$(mktemp -u /tmp/irc_fifo.XXXXXX)"
cleanup() {
  { exec 3>&-; } 2>/dev/null || true
  [[ -n "${NC_PID:-}" ]] && kill "$NC_PID" 2>/dev/null || true
  [[ -p "$FIFO" ]] && rm -f "$FIFO" || true
}
trap cleanup EXIT

mkfifo "$FIFO"

# Build best netcat command available
NC_CMD=(nc)
if nc -h 2>&1 | grep -qE '(^| )-C( |,|$)'; then NC_CMD+=(-C); fi
if nc -h 2>&1 | grep -qE '(^| )-q( |,|$)'; then NC_CMD+=(-q 0); fi   # exit on stdin EOF
if nc -h 2>&1 | grep -q -- '-w ';       then NC_CMD+=(-w 3);  fi

# Prefer ncat if present (handles CRLF reliably)
if command -v ncat >/dev/null 2>&1; then NC_CMD=(ncat --crlf); fi

# Start nc in background: it reads from FIFO, writes to terminal
"${NC_CMD[@]}" "$HOST" "$PORT" <"$FIFO" &
NC_PID=$!

sleep 0.12
if ! kill -0 "$NC_PID" 2>/dev/null; then
  echo "[irc_macro] Unable to connect to ${HOST}:${PORT}" >&2
  exit 1
fi

# Keep FIFO open for writing via FD 3
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

echo

# Close stdin to nc (if it supports -q 0, it'll exit immediately)
sleep 0.20
exec 3>&- 2>/dev/null || true

# If still alive, escalate: SIGINT → TERM → KILL
for sig in INT TERM KILL; do
  sleep 0.20
  kill -0 "$NC_PID" 2>/dev/null || break
  kill -"$sig" "$NC_PID" 2>/dev/null || true
done
wait "$NC_PID" 2>/dev/null || true
