SOURCES = main.c net.c v4l2.c
TARGET = ../output/cam.elf
CC = arm-linux-gnueabihf-gcc
CFLAGS = -Wall

$(TARGET):$(SOURCES)
	$(CC) $^ -o $@ $(CFLAGS)
