all:
	gcc src/main.c src/huffman.c src/drzewo.c -o huff

debug:
	gcc src/main.c src/huffman.c src/drzewo.c -o huff -Wall -Wextra -pedantic