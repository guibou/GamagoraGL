CXXFLAGS=-g -std=c++11 -I glad/include -Wall
LDFLAGS=-lGL -lX11 -lglfw
CC=g++

LD=g++

all: main

texture.o: texture.h texture.cpp
main.o: main.cpp texture.h

main: main.o texture.o glad/src/glad.c
