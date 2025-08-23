CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME     = ircserv
SRCS     = src/main.cpp \
		   src/Channel.cpp \
		   src/User.cpp \
		   src/Validation.cpp 
OBJS     = $(SRCS:src/%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
