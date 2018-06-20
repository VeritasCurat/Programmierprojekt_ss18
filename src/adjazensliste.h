#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int Element_liste[100000][2]; int EL_anz;
int Loesung_liste[100000][4];

int Raeume[100][100000][2]; int R_anz[100];

int anzKacheln;


extern void init();
extern int suche(int x, int y);
extern void listevergroessern();
extern void einfuegen(int x, int y);
extern int suche(int x, int y);
extern void printlist();
extern void printraeume();
extern void sort();
