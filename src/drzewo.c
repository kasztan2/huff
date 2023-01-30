#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "drzewo.h"

//? tworzy nowe drzewo (wierzchołek) z wartościami
tree nowe_drzewo(unsigned char wart, long long prawd)
{
    tree drzewko=malloc(sizeof(Node));
    drzewko->wart=wart;
    drzewko->prawd=prawd;
    drzewko->ma_wart=true;
    drzewko->lewy=NULL;
    drzewko->prawy=NULL;
    return drzewko;
}

//? tworzy nowe drzewo (wierzchołek) bez wartości
tree nowe_drzewo_puste()
{
    tree drzewko=malloc(sizeof(Node));
    drzewko->ma_wart=false;
    drzewko->lewy=NULL;
    drzewko->prawy=NULL;
    return drzewko;
}

lista nowa_lista(tree t)
{
    lista elem=malloc(sizeof(listElement));
    elem->next=NULL;
    elem->wart=t;
    return elem;
}

//? dodaje element (drzewo %t%) do listy %pocz%
lista dodaj_element(lista pocz, tree t)
{
    lista pocz_kopia=pocz;
    while(pocz->next!=NULL)pocz=pocz->next;
    pocz->next=nowa_lista(t);
    return pocz_kopia;
}

//? łączy dwa drzewa z największą sumą wystąpień, usuwa je z początku listy i dodaje w odpowiednim miejscu drzewo z połączonych drzew
lista drzewo_rosnie(lista drzewa)
{
    lista it=drzewa;

    tree lewy, prawy;
    lewy=it->wart;
    it=it->next;
    prawy=it->wart;
    it=it->next;

    tree nowe_drzewo=nowe_drzewo_puste();
    nowe_drzewo->ma_wart=false;
    nowe_drzewo->prawd=lewy->prawd+prawy->prawd;
    nowe_drzewo->lewy=(struct Node*)lewy;
    nowe_drzewo->prawy=(struct Node*)prawy;

    free(drzewa->next);
    free(drzewa);
    drzewa=it;

    while(it->next!=NULL&&(it->next->wart->prawd)<(nowe_drzewo->prawd))it=it->next;

    lista nast=it->next;
    lista nowy_element=nowa_lista(nowe_drzewo);
    it->next=nowy_element;
    nowy_element->next=nast;

    return drzewa;
}

//? zwraca długość listy %l%
int dlugosc_listy(lista l)
{
    int licznik=1;
    while(l->next!=NULL)
    {
        ++licznik;
        l=l->next;
    }
    return licznik;
}

//? usuwa drzewo, zwalnia pamięć (rekurencyjnie)
void usun_drzewo(tree drzewo)
{
    if(drzewo==NULL)return;
    if(drzewo->lewy!=NULL)usun_drzewo(drzewo->lewy);
    if(drzewo->prawy!=NULL)usun_drzewo(drzewo->prawy);
    free(drzewo);
}

//? usuwa listę, zwalnia pamięć (rekurencyjnie)
void usun_liste(lista l)
{
    if(l==NULL)return;
    if(l->next!=NULL)usun_liste(l->next);
    free(l);
}