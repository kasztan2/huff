#include "huffman.h"

int main(int argc, char **argv)
{
    if(argc<=2)
    {
        fprintf(stderr, "Nieprawidłowe polecenie!\n");
        return 0;
    }

    if(strcmp(argv[1], "encode")==0)zakoduj(argc-2, argv+2);
    else if(strcmp(argv[1], "decode")==0)odkoduj(argc-2, argv+2);
    else fprintf(stderr, "Zła opcja!\n");
}