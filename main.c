#include "huffman.h"

int main(int argc, char **argv)
{
    int opt;
    while((opt=getopt(argc, argv, "h"))!=-1)
    {
        switch(opt)
        {
            case 'h':
            {
                printf("Instrukcja:\n");
                printf("encode <plik wyjściowy> <plik wejściowy 1> <plik wejściowy 2> ...\n");
                printf("decode <nazwa pliku archiwum do rozpakowania>\n");
                return 0;
            }
            default:
            {
                printf("Instrukcja:\n");
                printf("encode <plik wyjściowy> <plik wejściowy 1> <plik wejściowy 2> ...\n");
                printf("decode <nazwa pliku archiwum do rozpakowania>\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    if(argc<=2)
    {
        fprintf(stderr, "Nieprawidłowe polecenie!\n");
        return 0;
    }

    if(strcmp(argv[1], "encode")==0)zakoduj(argc-3, argv, argv[2]);
    else if(strcmp(argv[1], "decode")==0)odkoduj(*(argv+2));
    else fprintf(stderr, "Zła opcja!\n");
}