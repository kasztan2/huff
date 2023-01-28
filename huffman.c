#include "huffman.h"
#include "drzewo.h"

int comp_znaki_prawd(const void *x, const void *y)
{
    znak *a=(znak*)x;
    znak *b=(znak*)y;
    if(a->prawd<b->prawd)return -1;
    if(a->prawd>b->prawd)return 1;
    return 0;
}

void policz_prawdopodobienstwa(znak prawd[256], char *nazwa_pliku)
{
    FILE *plik=fopen(nazwa_pliku, "r");
    char ch;
    while((ch=getc(plik))!=EOF)
    {
        ++prawd[ch].prawd;
    }
    fclose(plik);
}

void koduj(int ile_plikow, char **nazwy_plikow, char **kodowania, FILE *ptr)
{
    char b=0;
    char licznik=0;
    char ile_bitow_ostatniego_bajtu[ile_plikow];
    long long licznik_bajtow=0;
    long long ile_bajtow[ile_plikow];
    //printf("koduj\n");
    for(int i=0; i<ile_plikow; ++i)
    {
        FILE *plik=fopen(nazwy_plikow[i], "r");
        char ch;
        b=0;
        licznik=0;
        licznik_bajtow=0;
        //printf("plik\n");
        while((ch=getc(plik))!=EOF)
        {
            //printf("znak %c\n", ch);
            for(int i=0; i<258; ++i)
            {
                //printf("petla for %d %d\n", ch, i);
                if(kodowania[ch][i]=='\0')break;
                //printf("bbb\n");
                b*=2;
                if(kodowania[ch][i]=='1')b+=1;
                ++licznik;
                if(licznik==8)
                {
                    //printf("przed fwrite\n");
                    fwrite(&b, sizeof(char), 1, ptr);
                    //printf("po fwrite\n");
                    licznik=0;
                    b=0;
                    ++licznik_bajtow;
                }
            }
        }
        if(licznik>0)
        {
            fwrite(&b, sizeof(char), 1, ptr);
            ++licznik_bajtow;
        }
        fclose(plik);
        ile_bitow_ostatniego_bajtu[i]=licznik;
        ile_bajtow[i]=licznik_bajtow;
    }
    FILE *beginning_ptr=fopen("output", "wb");
    fwrite(&ile_plikow, sizeof(int), 1, ptr);
    for(int i=0; i<ile_plikow; ++i)
    {
        fwrite(&ile_bajtow[i], sizeof(long long), 1, beginning_ptr);
        fwrite(&ile_bitow_ostatniego_bajtu[i], sizeof(char), 1, beginning_ptr);
    }
    for(int i=0; i<128; ++i)
    {
        //printf("pocz %d\n", i);
        int suma=0, pot=1;
        for(char *ptr_kodowanie=kodowania[i]; (*ptr_kodowanie)!='\0'; ++ptr_kodowanie)
        {
            if(*ptr_kodowanie=='1')suma+=pot;
            pot*=2;
        }
        //printf("test\n");
        fwrite(&suma, sizeof(int), 1, beginning_ptr);
    }

    fclose(ptr);
    FILE *do_skopiowania=fopen("huffman.temp", "rb");

    char ch;
    while((ch=getc(do_skopiowania))!=EOF)
    {
        fwrite(&ch, sizeof(char), 1, beginning_ptr);
    }

    fclose(beginning_ptr);
    remove("huffman.temp");
}

void wpisz_kodowania(tree drzewko, char **kodowania, char *sciezka, int *dl_sciezki)
{
    //printf("%s\n", sciezka);
    if(drzewko->ma_wart)
    {
        //printf("znak do kodowania: %c\n", drzewko->wart);
        kodowania[drzewko->wart]=calloc(sizeof(char), 260);
        strcpy(kodowania[drzewko->wart], sciezka);
        sciezka[*dl_sciezki]='\0';
        --*dl_sciezki;
        return;
    }
    sciezka[*dl_sciezki]='0';
    (*dl_sciezki)++;
    wpisz_kodowania((tree)drzewko->lewy, kodowania, sciezka, dl_sciezki);
    sciezka[*dl_sciezki]='1';
    (*dl_sciezki)++;
    wpisz_kodowania((tree)drzewko->prawy, kodowania, sciezka, dl_sciezki);

    sciezka[*dl_sciezki]='\0';
    (*dl_sciezki)--;
}

void zakoduj(int ile_plikow, char **nazwy_plikow)
{
    znak znaki_prawd[256];
    for(int i=0; i<256; ++i){znaki_prawd[i].prawd=0;znaki_prawd[i].c=i;}
    for(int i=0; i<ile_plikow; ++i)policz_prawdopodobienstwa(znaki_prawd, *nazwy_plikow+i);
    qsort(znaki_prawd, 256, sizeof(znak), comp_znaki_prawd);
    //printf("%lld\n", znaki_prawd[0].prawd);
    lista drzewa=nowa_lista(nowe_drzewo(znaki_prawd[0].c, znaki_prawd[0].prawd));
    for(int i=1; i<256; ++i)
    {
        //if(znaki_prawd[i].prawd==0)break;
        drzewa=dodaj_element(drzewa, nowe_drzewo(znaki_prawd[i].c, znaki_prawd[i].prawd));
    }
    for(int i=0; i<254; ++i)
    {
        drzewa=drzewo_rosnie(drzewa);
    }
    //printf("dl listy drzewa: %d\n", dlugosc_listy(drzewa));
    printf("ten printf jakoś powoduje, że program nie wywala segfaulta :D\n");
    tree drzewko_huffmana=nowe_drzewo_puste();
    drzewko_huffmana->lewy=(struct Node*)drzewa->wart;
    drzewko_huffmana->prawy=(struct Node*)drzewa->next->wart;
    drzewko_huffmana->prawd=drzewa->wart->prawd;
    drzewko_huffmana->prawd+=drzewa->next->wart->prawd;
    //free(drzewa->next);
    //free(drzewa);

    char **kodowania=calloc(sizeof(char*), 260);
    char *sciezka=calloc(sizeof(char), 260);
    sciezka[0]='\0';
    int dl_sciezki;
    dl_sciezki=0;
    wpisz_kodowania(drzewko_huffmana, kodowania, sciezka, &dl_sciezki);
    
    FILE *output=fopen("huffman.temp", "wb");
    koduj(ile_plikow, nazwy_plikow, kodowania, output);

    //fclose(output);
}

void odkoduj(int argc, char *argv)
{

}