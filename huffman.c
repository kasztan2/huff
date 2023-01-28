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

void policz_prawdopodobienstwa(znak prawd[256], unsigned char *nazwa_pliku)
{
    FILE *plik=fopen(nazwa_pliku, "r");
    char ch;
    while((ch=getc(plik))!=EOF)
    {
        ++prawd[ch].prawd;
    }
    fclose(plik);
}

void koduj(int ile_plikow, unsigned char **nazwy_plikow, unsigned char **kodowania, FILE *ptr, char *plik_wyj)
{
    unsigned char b=0, pot=1;
    unsigned char licznik=0;
    unsigned char ile_bitow_ostatniego_bajtu[ile_plikow];
    long long licznik_bajtow=0;
    long long ile_bajtow[ile_plikow];

    for(int i=0; i<ile_plikow; ++i)
    {
        FILE *plik=fopen(nazwy_plikow[i], "r");
        char ch;
        b=0;
        licznik=0;
        licznik_bajtow=0;

        char ile_znakow_w_nazwie=strlen(nazwy_plikow[i]);
        fwrite(&ile_znakow_w_nazwie, sizeof(char), 1, ptr);
        fwrite(nazwy_plikow[i], sizeof(char), ile_znakow_w_nazwie, ptr);

        while((ch=getc(plik))!=EOF)
        {
            for(int i=0; i<258; ++i)
            {
                if(kodowania[ch][i]=='\0')break;
                if(kodowania[ch][i]=='1')b+=pot;
                ++licznik;
                pot*=2;
                if(licznik==8)
                {
                    fwrite(&b, sizeof(unsigned char), 1, ptr);
                    licznik=0;
                    b=0;
                    pot=1;
                    ++licznik_bajtow;
                }
            }
        }
        if(licznik>0)
        {
            fwrite(&b, sizeof(unsigned char), 1, ptr);
            ++licznik_bajtow;
        }
        fclose(plik);
        ile_bitow_ostatniego_bajtu[i]=licznik;
        ile_bajtow[i]=licznik_bajtow;
    }
    FILE *beginning_ptr=fopen(plik_wyj, "wb");
    fwrite(&ile_plikow, sizeof(int), 1, beginning_ptr);

    for(int i=0; i<ile_plikow; ++i)
    {
        fwrite(&ile_bajtow[i], sizeof(long long), 1, beginning_ptr);
        fwrite(&ile_bitow_ostatniego_bajtu[i], sizeof(unsigned char), 1, beginning_ptr);
    }
    for(int i=0; i<256; ++i)
    {
        unsigned int suma=0, pot=1;
        char it=0;
        for(char *ptr_kodowanie=kodowania[i]; (*ptr_kodowanie)!='\0'; ++ptr_kodowanie)
        {
            if(*ptr_kodowanie=='1')suma+=pot;
            pot*=2;
            ++it;
        }

        fwrite(&it, sizeof(char), 1, beginning_ptr);
        fwrite(&suma, sizeof(unsigned int), 1, beginning_ptr);
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

void wpisz_kodowania(tree drzewko, unsigned char **kodowania, unsigned char *sciezka, int *dl_sciezki)
{

    if(drzewko->ma_wart)
    {

        kodowania[drzewko->wart]=calloc(sizeof(unsigned char), 260);
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

void zakoduj(int ile_plikow, unsigned char **nazwy_plikow, char *plik_wyj)
{
    znak znaki_prawd[256];
    for(int i=0; i<256; ++i){znaki_prawd[i].prawd=0;znaki_prawd[i].c=i;}
    for(int i=0; i<ile_plikow; ++i)policz_prawdopodobienstwa(znaki_prawd, *nazwy_plikow+i);
    qsort(znaki_prawd, 256, sizeof(znak), comp_znaki_prawd);

    lista drzewa=nowa_lista(nowe_drzewo(znaki_prawd[0].c, znaki_prawd[0].prawd));
    for(int i=1; i<256; ++i)
    {

        drzewa=dodaj_element(drzewa, nowe_drzewo(znaki_prawd[i].c, znaki_prawd[i].prawd));
    }
    for(int i=0; i<254; ++i)
    {
        drzewa=drzewo_rosnie(drzewa);
    }
    tree drzewko_huffmana=nowe_drzewo_puste();
    drzewko_huffmana->lewy=(struct Node*)drzewa->wart;
    drzewko_huffmana->prawy=(struct Node*)drzewa->next->wart;
    drzewko_huffmana->prawd=drzewa->wart->prawd;
    drzewko_huffmana->prawd+=drzewa->next->wart->prawd;

    unsigned char **kodowania=calloc(sizeof(char*), 260);
    unsigned char *sciezka=calloc(sizeof(char), 260);
    sciezka[0]='\0';
    int dl_sciezki;
    dl_sciezki=0;
    wpisz_kodowania(drzewko_huffmana, kodowania, sciezka, &dl_sciezki);
    
    FILE *output=fopen("huffman.temp", "wb");
    koduj(ile_plikow, nazwy_plikow, kodowania, output, plik_wyj);

    free(kodowania);
    free(sciezka);
}

void odkoduj(unsigned char *nazwa_pliku)
{
    FILE *ptr=fopen(nazwa_pliku, "rb");

    int liczba_plikow;
    fread(&liczba_plikow, sizeof(int), 1, ptr);

    long long ile_bajtow[liczba_plikow];
    char ile_bitow_na_koncu[liczba_plikow];
    for(int i=0; i<liczba_plikow; ++i)
    {
        fread(&ile_bajtow[i], sizeof(long long), 1, ptr);
        fread(&ile_bitow_na_koncu[i], sizeof(char), 1, ptr);
    }

    tree drzewko=nowe_drzewo_puste();
    tree it=drzewko;
    unsigned int kodowanie;

    for(int i=0; i<256; ++i)
    {
        char dl;
        fread(&dl, sizeof(char), 1, ptr);
        fread(&kodowanie, sizeof(int), 1, ptr);

        it=drzewko;
        while(dl--)
        {
            if(kodowanie%2)
            {
                if(it->lewy==NULL)it->lewy=nowe_drzewo_puste();
                it=it->lewy;
            }
            else
            {
                if(it->prawy==NULL)it->prawy=nowe_drzewo_puste();
                it=it->prawy;
            }
            kodowanie/=2;
        }
        it->ma_wart=true;
        it->wart=(unsigned char)i;
    }

    unsigned char wej;
    it=drzewko;
    for(int i=0; i<liczba_plikow; ++i)
    {
        char dl_nazwy;
        fread(&dl_nazwy, sizeof(char), 1, ptr);
        char nazwa_pliku[dl_nazwy+1];
        nazwa_pliku[dl_nazwy]='\0';
        for(int j=0; j<dl_nazwy; ++j)
        {
            fread(&nazwa_pliku[j], sizeof(char), 1, ptr);
        }

        FILE *plik_wyj=fopen(nazwa_pliku, "w");
        for(int nr_bajtu=0; nr_bajtu<ile_bajtow[i]-1; ++nr_bajtu)
        {

            fread(&wej, sizeof(unsigned char), 1, ptr);
            for(int bit=0; bit<8; ++bit)
            {
                if(wej%2==1)it=it->lewy;
                else it=it->prawy;
                if(it->ma_wart)
                {
                    fprintf(plik_wyj, "%c", it->wart);
                    it=drzewko;
                }
                wej/=2;
            }
        }
        fread(&wej, sizeof(unsigned char), 1, ptr);
        for(int bit=0; bit<ile_bitow_na_koncu[i]; ++bit)
        {
            if(wej%2==1)it=it->lewy;
            else it=it->prawy;
            if(it->ma_wart)
            {
                fprintf(plik_wyj, "%c", it->wart);
                it=drzewko;
            }
            wej/=2;
        }
        fclose(plik_wyj);
    }

    fclose(ptr);
}