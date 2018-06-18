#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t liste_size = 100;
int position=0;
int** Element_liste;

void init();
int suche(int x, int y);
void listevergroessern();
void einfuegen(int x, int y);
