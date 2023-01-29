#include "huffman.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    bool stats=false;
    bool o_flag=false;
    bool debug_flag=false;
    int opt;
    char *plik_wyj=NULL;
    while((opt=getopt(argc, argv, "so:dh"))!=-1)
    {
        switch(opt)
        {
            case 's':
            {
                printf("Statystyki:\n");
                stats=true;
                break;
            }
            case 'o':
            {
                plik_wyj=optarg;
                o_flag=true;
                break;
            }
            case 'd':
            {
                debug_flag=true;
                break;
            }
            case 'h':
            default:
            {
                printf("Instrukcja:\n");
                printf("encode -o <plik wyjściowy> <plik wejściowy 1> <plik wejściowy 2> ...\n");
                printf("decode <nazwa pliku archiwum do rozpakowania>\n");
                exit(EXIT_SUCCESS);
            }
        }
    }

    if(plik_wyj==NULL)
    {
        plik_wyj=calloc(sizeof(char), 10);
        plik_wyj="arch.huff";
    }

    if(optind>=argc)
    {
        fprintf(stderr, "Brakujący argument! Użyj -h, aby wyświetlić pomoc\n");
        exit(EXIT_FAILURE);
    }

    if(debug_flag)
    {
        for(int i=0; i<argc; ++i)
        {
            printf("%s\n", argv[i]);
        }
        printf("%d %d\n", argc, optind);
        printf("nazwa pliku wyjściowego: %s\n", plik_wyj);
    }

    if(strcmp(argv[optind], "encode")==0)
    {
        zakoduj(optind+1, argc-o_flag*2-stats-2-debug_flag, argv, plik_wyj, stats);
    }
    else if(strcmp(argv[optind], "decode")==0)
    {
        odkoduj(*(argv+optind+1));
    }
    else fprintf(stderr, "Złe polecenie! Użyj -h, aby wyświetlić pomoc\n");
}