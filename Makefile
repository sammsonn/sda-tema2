CC=gcc
DEPS=quadtree.c
EXE=quadtree

build: $(DEPS)
	$(CC) -Wall $^ -o $(EXE) -g

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -f $(EXE)