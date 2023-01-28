#include "huffman.h"

int main(int argc, unsigned char **argv)
{
    if(argc<=2)
    {
        fprintf(stderr, "Nieprawidłowe polecenie!\n");
        return 0;
    }

    if(strcmp(argv[1], "encode")==0)zakoduj(argc-3, argv+3, argv[2]);
    else if(strcmp(argv[1], "decode")==0)odkoduj(*(argv+2));
    else fprintf(stderr, "Zła opcja!\n");
}