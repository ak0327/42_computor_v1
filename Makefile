NAME		= computor

CXX			= c++
CXXFLAGS	= -std=c++20 -Wall -Wextra -Werror -MMD -MP -pedantic

SRCS_DIR	= srcs
SRCS		= main.cpp \
			  computor.cpp \
			  Calculator/Calculator.cpp \
			  Parser/Parser.cpp \
			  Tokenizer/Tokenizer.cpp

OBJS_DIR	= objs
OBJS		= $(SRCS:%.cpp=$(OBJS_DIR)/%.o)
DEPS		= $(OBJS:%.o=%.d)

INCL_DIR 	= srcs \
			  srcs/Calculator \
			  srcs/Parser \
			  srcs/Result \
			  srcs/Tokenizer

INCLUDES	= $(addprefix -I, $(INCL_DIR))

.PHONY	: all
all		: $(NAME)

$(NAME)	: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJS_DIR)/%.o	: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY	: clean
clean	:
	rm -rf $(OBJS_DIR)

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
	#./build/utest --gtest_filter=TestTokenizer.*
	#./build/utest --gtest_filter=TestParser.TestParseTermOK
	#./build/utest --gtest_filter=TestParser.TestParseEquation*
	#./build/utest --gtest_filter=TestParser.TestReducedForm
	#./build/utest --gtest_filter=*TestComputor.*
	#./build/utest --gtest_filter=TestLib.*

-include $(DEPS)
