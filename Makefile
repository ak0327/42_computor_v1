NAME		= computor

CXX			= c++
CXXFLAGS	= -std=c++20 -Wall -Wextra -Werror -MMD -MP -pedantic

SRCS		= main.cpp

OBJ_DIR		= objs
OBJS		= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS		= $(OBJS:%.o=%.d)

INCLUDES	= -I.

all	: $(NAME)

$(NAME)	: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o	: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

fclean	:
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re		: fclean all

run		: re
	./$(NAME)



-include $(DEPS)
