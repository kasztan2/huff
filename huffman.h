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
    char c;
} znak;

void policz_prawdopodobienstwa(znak prawd[256], char *nazwa_pliku);
void zakoduj(int ile_plikow, char **nazwy_plikow);
void odkoduj(int argc, char *argv);
#endif