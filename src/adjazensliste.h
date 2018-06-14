#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Element
{
	int X;
	int Y;
};


int groesse=0;

size_t liste_size = 100;
const size_t element_size = sizeof(struct Element);
struct element ** element_liste;

void init();
int suche(struct Element *a);
void listevergroessern_();
void append(struct Element *a);
void einfuegen(struct Element *a);
