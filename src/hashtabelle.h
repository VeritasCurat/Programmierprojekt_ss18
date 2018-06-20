#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int P = 999999; //TODO: primzahl in dem Bereich finden
int belegt = 0;
int maxNEXT = 10;

int Hash_liste[P][2];

extern H_init();
extern H_eintragen(int x, int y);
extern H_suchen(int x, int y);
