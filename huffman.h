#ifndef HUFFMAN
#define HUFFMAN
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "drzewo.h"

typedef struct znak
{
    long long prawd;
    unsigned char c;
} znak;

void policz_prawdopodobienstwa(znak prawd[256], unsigned char *nazwa_pliku);
void zakoduj(int odkad, int ile_plikow, char **nazwy_plikow, char *plik_wyj, bool stats);
void odkoduj(unsigned char *nazwa_pliku);
#endif