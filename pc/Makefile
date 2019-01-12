SOURCES = sdl.c
TARGET = ../output/play.elf
CC = gcc
CFLAGS = -Wall

$(TARGET):$(SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) `pkg-config sdl2 --cflags --libs`
