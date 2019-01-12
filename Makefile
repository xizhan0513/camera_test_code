CAMERA = ./board
PLAY = ./pc

all:
	make -C $(CAMERA)
	make -C $(PLAY)

clean:
	rm ./output/*
