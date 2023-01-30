#ifndef DRZEWO
#define DRZEWO
#include <stdbool.h>

typedef struct Node
{
    bool ma_wart;//? czy ma wartości w sobie
    unsigned char wart;//? jaki ~znak
    long long prawd;//? ilość wystąpień ~znaku
    struct Node *lewy, *prawy;//? wskaźniki na dzieci
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

void usun_drzewo(tree drzewo);
void usun_liste(lista l);

#endif