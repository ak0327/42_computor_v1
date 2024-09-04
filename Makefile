NAME		= computor

CXX			= c++
CXXFLAGS	= -std=c++20 -Wall -Wextra -Werror -MMD -MP -pedantic

SRCS		= main.cpp

OBJ_DIR		= objs
OBJS		= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS		= $(OBJS:%.o=%.d)

INCLUDES	= -I.

.PHONY	: all
all		: $(NAME)

$(NAME)	: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o	: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY	: clean
clean	:
	rm -rf $(OBJ_DIR)

.PHONY	: fclean
fclean	: clean
	rm -f $(NAME)

.PHONY	: re
re		: fclean all

.PHONY	: lint
lint	:
	@python3 -m cpplint --recursive srcs \
	&& echo "\033[0;32mCPPLINT DONE\033[0m" \
	|| echo "\033[0;31mCPPLINT ERROR\033[0m"

.PHONY	: utest
utest	:
	cmake -S . -B build
	cmake --build build
	./build/utest

-include $(DEPS)
