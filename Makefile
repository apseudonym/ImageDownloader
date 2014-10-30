# Files to compile
OBJS = ./src/*.c

# Compiler
CC = gcc

# Flags
COMPILER_FLAGS = -Wall

# Executable Path
OBJ_PATH = ./bin/

# Executable Name
OBJ_NAME = imgdownloader

all: $(OBJS)
		mkdir -p $(OBJ_PATH)
		$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_PATH)$(OBJ_NAME)

clean: 
		rm -rf $(OBJ_PATH)
