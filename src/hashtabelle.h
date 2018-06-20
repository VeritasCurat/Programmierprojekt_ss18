#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int H_belegt = 0;
int Hash_liste[999983][2];
int P = 999983;

extern void H_eintragen(int x, int y);
extern int H_suchen(int x, int y);
