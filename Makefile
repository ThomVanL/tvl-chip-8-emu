PROGRAM_NAME := chip8emu

SDLFLAGS = $(shell sdl2-config --libs --cflags)

CPPFLAGS= -I ./inc/CHIP8
CFLAGS=-std=c11 -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs
OBJECTS=./build/CHIP8.o ./build/CHIP8_memory.o ./build/CHIP8_stack.o ./build/CHIP8_keyboard.o ./build/CHIP8_screen.o

ifeq ($(OS),Windows_NT)
    DETECTED_OS = Windows
	PROGRAM_NAME += .exe
else
    DETECTED_OS = $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

.PHONY: all clean

all: clean ${OBJECTS}
	gcc ${CFLAGS} ${CPPFLAGS} ./src/main.c ${OBJECTS} ${SDLFLAGS} -o ./bin/${PROGRAM_NAME}

./build/CHIP8.o:./src/CHIP8.c
	gcc ${CFLAGS} ${CPPFLAGS} ./src/CHIP8.c -c -o ./build/CHIP8.o

./build/CHIP8_memory.o:./src/CHIP8_memory.c
	gcc ${CFLAGS} ${CPPFLAGS} ./src/CHIP8_memory.c -c -o ./build/CHIP8_memory.o

./build/CHIP8_stack.o:./src/CHIP8_stack.c
	gcc ${CFLAGS} ${CPPFLAGS} ./src/CHIP8_stack.c -c -o ./build/CHIP8_stack.o

./build/CHIP8_keyboard.o:./src/CHIP8_keyboard.c
	gcc ${CFLAGS} ${CPPFLAGS} ./src/CHIP8_keyboard.c -c -o ./build/CHIP8_keyboard.o

./build/CHIP8_screen.o:./src/CHIP8_screen.c
	gcc ${CFLAGS} ${CPPFLAGS} ./src/CHIP8_screen.c -c -o ./build/CHIP8_screen.o

clean:
	@echo 'Detected OS:' $(DETECTED_OS)
ifeq ($(DETECTED_OS),Windows)
	del bin\*
	del build\*
else
	rm -rf bin/*
	rm -rf build/*
endif