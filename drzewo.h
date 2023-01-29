#ifndef DRZEWO
#define DRZEWO
#include <stdbool.h>

typedef struct
{
    bool ma_wart;
    unsigned char wart;
    long long prawd;
    struct Node *lewy, *prawy;
} Node;

typedef Node* tree;

tree nowe_drzewo(unsigned char wart, long long prawd);
tree nowe_drzewo_puste();



typedef struct listElem
{
    tree wart;
    struct listElem *next;
} listElement;

typedef listElement* lista;

lista nowa_lista(tree t);
lista dodaj_element(lista pocz, tree t);
lista drzewo_rosnie(lista drzewa);
int dlugosc_listy(lista l);

#endif