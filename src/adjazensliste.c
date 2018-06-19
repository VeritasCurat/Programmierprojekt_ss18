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

void printraeume(){
	int index =0;
	while(R_anz[index] != 0){
		printf("Raum %d:\n",index);
		for(int i=0; i<R_anz[index]; i++){
			printf("x: %d, y: %d\n", Raeume[index][i][0], Raeume[index][i][1]);
		}

		++index;
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


	//printlist();

	Element_liste[position][0] = x;
	Element_liste[position][1] = y;

	++position;
}

void test_al(){
	init();

	einfuegen(5,1);

	printlist();
}

static int comp(const void* a, const void* b) {
  int* array1 = (int*) a;
  int* array2 = (int*) b;
  int diff1 = array1[0] - array2[0];
  if (diff1) return diff1;
  return array1[1] - array2[1];
}

void sort(){
	 qsort(Element_liste, position, 2*sizeof(int), comp);
}

/*
int main(){
	einfuegen(10,2);
	einfuegen(5,1);
	einfuegen(2,0);
	einfuegen(5,2);
	sort();
	printlist();
}
*/
