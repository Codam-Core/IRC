NAME		:= ircserv

CXX			:= c++
CXXFLAGS	:= -std=c++23 -Wall -Wextra 
# CXXFLAGS	+= -Werror -g -fsanitize=address

SRC_DIR		:= src
INCL_DIR	:= incl
OBJ_DIR		:= obj

INCL		:=	colours.hpp \
				Server.hpp \
				Fd.hpp
				
SRCS		:=	main.cpp \
				Server.cpp \
				Fd.cpp


# Add the folder to every file name
OBJS		:= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
HEADERS		:= $(INCL:%=$(INCL_DIR)/%)

# Default rule (compiles the program)
all: $(NAME)

# Linking the final executable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compiling source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I $(INCL_DIR) -c $< -o $@

# Cleaning compiled files
clean:
	rm -rf $(OBJ_DIR)

# Cleaning everything (object files + executable)
fclean: clean
	rm -f $(NAME)

# Recompile everything
re: fclean all

# Compile and run the program
run: all
	./$(NAME) 6667 secret

.PHONY: all clean fclean re run
