NAME		:= ircserv

CXX			:= c++
CXXFLAGS	:= -std=c++23 -Wall -Wextra 
# CXXFLAGS	+= -Werror -g -fsanitize=address

SRC_DIR		:= src
INCL_DIR	:= incl
OBJ_DIR		:= obj

INCL		:=	$(notdir $(wildcard $(INCL_DIR)/*.hpp))
SRCS		:=	$(notdir $(wildcard $(SRC_DIR)/*.cpp))

# Add the folder to every file name
OBJS		:= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
HEADERS		:= $(INCL:%=$(INCL_DIR)/%)

# Default rule (compiles the program)
all: $(NAME)

# Linking the final executable
$(NAME): $(OBJS)
	@printf "\r\033[K linking    $(NAME)"
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@printf "\r\033[K ✔          Program ready!\n"

# Compiling source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "\r\033[K compiling  $(notdir $<)"
	@$(CXX) $(CXXFLAGS) -I $(INCL_DIR) -c $< -o $@

# Cleaning compiled files
clean:
	@rm -rf $(OBJ_DIR)
	@printf " ✔          removed $(OBJ_DIR)/\n"

# Cleaning everything (object files + executable)
fclean: clean
	@rm -f $(NAME)
	@printf " ✔          removed $(NAME)\n"

# Recompile everything
re: fclean all

# Compile and run the program
run: all
	./$(NAME) 6667 secret

vrun: all
	valgrind --track-fds=yes ./$(NAME) 6667 secret

.PHONY: all clean fclean re run vrun
