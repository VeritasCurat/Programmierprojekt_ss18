/*
 * adjazensliste.c
 *
 *  Created on: 07.06.2018
 *      Author: Johannes
 */


#include "adjazensliste.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int position=0;
size_t liste_size = 10000;


void init(){

//	int (*Element_liste)[2] = malloc(sizeof(int[liste_size][2])); //TODO free(Element_liste)

	for(int i=0; i<liste_size; i++){
		Element_liste[i][0] = -1; Element_liste[i][1] = -1;
	}

}

void printlist(){
	for(int i=0; i<position; i++){
				printf("%d: A: %d, B: %d\n", i, Element_liste[i][0] , Element_liste[i][1]);
	}
}

int suche(int x, int y){
	for(int i=0; i< liste_size; i++){
		if(Element_liste[i][0] == x && Element_liste[i][1] == y)return i;
	}
	return -1;
}

void delete(int x, int y){
		int suche_ = suche(x,y);
		if(suche > 0){
			Element_liste[suche_][0]=-1;
			Element_liste[suche_][1]=-1;
		}
}

/*
void listevergroessern(){
	liste_size *= 2;
	Element_liste = (int **)  realloc(Element_liste, (liste_size * 2) * sizeof(int)*2);
}
*/

void einfuegen(int x, int y){

	/*
	if(position == liste_size){
		listevergroessern();
	}
	*/


	printlist();

	Element_liste[position][0] = x;
	Element_liste[position][1] = y;

	++position;
}

void test_al(){
	init();

  einfuegen(5,1);

	printlist();
}

void sort(){

}



int main(){
	test_al();
}
