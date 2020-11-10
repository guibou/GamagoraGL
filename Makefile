CFLAGS=-g -I glad/include -Wall -Wextra -isystem glad/include
CXXFLAGS=$(CFLAGS) -std=c++17 -Weffc++ -isystem Cimg/include -isystem tinyply/include
LDFLAGS=-lGL -lX11 -lglfw -ldl -lpthread -isystem glad/include
CC=g++

LD=g++

all: main

texture.o: texture.h texture.cpp
main.o: main.cpp texture.h

# This is incorrect, but you are not supposed to change anything in
# the glad directory.
glad/src/glad.o: glad/src/glad.c

main: main.o texture.o glad/src/glad.o
