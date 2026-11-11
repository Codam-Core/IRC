NAME		:= ircserv
TEST_NAME   := ircserv_test

CXX			:= c++
CXXFLAGS	:= -std=c++23 -Wall -Wextra 
# CXXFLAGS	+= -Werror -g -fsanitize=address

DEPFLAGS	:= -MMD -MP

SRC_DIR		:= src
INCL_DIR	:= incl
OBJ_DIR		:= obj
TESTS_DIR	:= tests

SRCS 		:=  $(patsubst $(SRC_DIR)/%,%,$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/commands/*.cpp))
TEST_SRCS	:=	$(notdir $(wildcard $(TESTS_DIR)/*.cpp))

# Exclude the regular main.cpp when building tests
TEST_APP_SRCS := $(filter-out main.cpp, $(SRCS))

# Add the folder to every file name
OBJS		:= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS   := $(TEST_APP_SRCS:%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS   += $(TEST_SRCS:%.cpp=$(OBJ_DIR)/tests/%.o)


# Default rule (compiles the program)
all: $(NAME)

# Compile and run tests
test: $(TEST_NAME)
	@./$(TEST_NAME)

# Linking the final executable
$(NAME): $(OBJS)
	@printf "\r\033[K linking    $(NAME)"
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@printf "\r\033[K ✔          Program ready!\n"

# Link the test executable
$(TEST_NAME): $(TEST_OBJS)
	@printf "\r\033[K linking    $(TEST_NAME)"
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@printf "\r\033[K ✔          Tests ready!\n"

# Compiling source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@printf "\r\033[K compiling  $(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) -I $(INCL_DIR) -c $< -o $@

# Compile test source files
$(OBJ_DIR)/tests/%.o: $(TESTS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@printf "\r\033[K compiling  $(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) -I $(INCL_DIR) -I $(TESTS_DIR) -c $< -o $@

# Cleaning compiled files
clean:
	@rm -rf $(OBJ_DIR)
	@printf " ✔          removed $(OBJ_DIR)/\n"

# Cleaning everything
fclean: clean
	@rm -f $(NAME) $(TEST_NAME)
	@printf " ✔          removed $(NAME) and $(TEST_NAME)\n"

# Recompile everything
re: fclean all

# Compile and run the program
run: all
	./$(NAME) 6667 secret

vrun: all
	valgrind --track-fds=yes ./$(NAME) 6667 secret

.PHONY: all test clean fclean re run vrun

# read the header dependencies the compiler wrote (-MMD); '-' = no error if they don't exist yet
-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)
