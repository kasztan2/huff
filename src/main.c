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
                printf("encode <plik wejściowy 1> <plik wejściowy 2> ...\n");
                printf("decode <nazwa pliku archiwum do rozpakowania>\n");
                printf("Opcje:\n-o <nazwa pliku>    określ nazwę pliku wyjściowego (archiwum), tylko do encode\n-d                  wypisuj dodatkowe informacje do debugowania\n-s                  wypisz statystyki, tylko do encode\n-h                  pomoc\n");
                exit(EXIT_SUCCESS);
            }
        }
    }

    if(optind>=argc-1)
    {
        fprintf(stderr, "🔭 Brakujący argument (nawet w kosmosie go nie widać)! Użyj %s -h, aby wyświetlić pomoc ℹ️\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(plik_wyj==NULL&&strcmp(argv[optind], "encode")==0)
    {
        plik_wyj=calloc(sizeof(char), 10);
        plik_wyj="arch.huff";
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
        if(stats)printf("Statystyki 📈:\n");
        zakoduj(optind+1, argc-o_flag*2-stats-2-debug_flag, argv, plik_wyj, stats);
    }
    else if(strcmp(argv[optind], "decode")==0)
    {
        if(stats||o_flag)
        {
            fprintf(stderr, "😳 Niedozwolona opcja!\n");
            exit(EXIT_FAILURE);
        }
        odkoduj(*(argv+optind+1));
    }
    else fprintf(stderr, "❌ Złe polecenie! Użyj %s -h, aby wyświetlić pomoc ℹ️\n", argv[0]);

    if(optarg!=NULL)free(optarg);
    exit(EXIT_SUCCESS);
}