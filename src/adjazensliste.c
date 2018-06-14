/*
 * adjazensliste2.c
 *
 *  Created on: 07.06.2018
 *      Author: Johannes
 */


#include "adjazensliste.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void init_(){
	element_liste = (struct Element **) malloc(100 * element_size);
}


int suche_(struct Element *a){
	for(int i=0; groesse; i++){
		struct Element b; b->X = element_liste[i]->X; b->Y = element_liste[i]->Y;
		if(a->X == b->X && a->Y == b->Y)return 1;
	}
	return 0;
}

void listevergroessern_(){
	liste_size *= 2;
	element_liste = (struct Element **)  realloc(element_liste, (liste_size * 2) * sizeof(struct Element *));
}

void einfuegen(struct Element *a){
	if(groesse == sizeof(element_liste)/sizeof(element_liste[0])-1){
		listevergroessern_();
	}
	element_liste[groesse] = a;
	++groesse;
}

void test_al(){
	init_();
	struct Element *a; a->X = 0; a->Y=0;


}

