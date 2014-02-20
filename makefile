# Which compiler?
CC = gcc

# Flags for the compiler.
FLAGS = -Wall

# The build directory.
BUILD_DIRECTORY = build

# The directory where source code is kept.
SOURCE = src

# The files to compile.
FILES = $(SOURCE)/assets.c $(SOURCE)/utilities.c $(SOURCE)/processing.c $(SOURCE)/logging.c

# The executable to create.
OUTPUT = $(BUILD_DIRECTORY)/assets

# Compile the executable.
build: $(FILES)
	@mkdir -p $(BUILD_DIRECTORY)
	@$(CC) $(FLAGS) -o $(OUTPUT) $(FILES)

# Clean up the files for a fresh start.
clean:
	rm -fr $(BUILD_DIRECTORY)

# Clean and build.
rebuild: clean build

# Install
install:
	@echo "Installing..."
	@echo "-- install assets /usr/local/bin"
	@sudo install $(OUTPUT) /usr/local/bin
	@echo "-- done"
