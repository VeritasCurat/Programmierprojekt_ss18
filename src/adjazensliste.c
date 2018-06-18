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


void init(){
/*
	Element_liste = (int**) malloc(liste_size * sizeof(int)*2);
	printf("TEST\n");
*/
	Element_liste[100][2];
	for(int i=0; i<liste_size; i++){
		Element_liste[i][0] = -1; Element_liste[i][1] = -1;
	}
}

void printlist(){
	for(int i=0; i<liste_size; i++){
			printf("%d: A: %d, B: %d\n", i, Element_liste[i][0] , Element_liste[i][1]);
	}
}

int suche(int x, int y){
	for(int i=0; liste_size; i++){
		if(Element_liste[i][0] == x && Element_liste[i][1] == y)return i;
	}
	return -1;
}

void delete(int x, int y){
		int suche = suche(x,y);
		if(suche > 0){
			Element_liste[suche][0]=-1;
			Element_liste[suche][1]=-1;
		}
}

void listevergroessern(){
	liste_size *= 2;
	Element_liste = (int **)  realloc(Element_liste, (liste_size * 2) * sizeof(int)*2);
}

void einfuegen(int x, int y){
	if(liste_size == sizeof(Element_liste)/sizeof(Element_liste[0])-1){
		listevergroessern();
	}
	++position;
}

void test_al(){
	init();

	einfuegen(5,1);

	printlist();

}

int main(){
	test_al();
}
