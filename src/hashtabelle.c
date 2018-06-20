/*
 * hashtabelle.c
 *
 *  Created on: 20.06.2018
 *      Author: Johannes
 */

int maxNEXT = 10;

int H_belegt = 0;
int Hash_liste[999983][2];
int P = 999983;

//HS: (x,y) -> 1000*x + 10*y % P

void H_init(){
	for(int i=0; i<P; i++){
		Hash_liste[i][0] = -1; Hash_liste[i][1] = -1;
	}
}

void H_eintragen(int x, int y){
	if(H_belegt == P){
		printf("Hashtabelle voll!");
		exit(-1);
	}
	int k =1000*x + 10*y %P;
	if(Hash_liste[k][0] == -1 && Hash_liste[k][1] ==-1){
		Hash_liste[k][0] = x; Hash_liste[k][1] = y;
	}
	else {
		//inplace hashing
		int next=2;
		while(Hash_liste[k*next*next % P][0] == -1 && Hash_liste[k*next*next % P][1] ==-1){
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("Hashtabelle zu dicht belegt!");
				exit(-1);
			}
			++next;
		}
		Hash_liste[k*next*next % P][0] = x; Hash_liste[k*next*next % P][1] = y;

	}
}

int H_suchen(int x, int y){
	int k =1000*x + 10*y %P;
	if(Hash_liste[k][0] == x && Hash_liste[k][1] ==y){
		return k;
	}
	else {
		//inplace hashing
		int next=2;
		while(Hash_liste[k*next*next % P][0] != x && Hash_liste[k*next*next % P][1] != y){
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				return 0;
			}
			++next;
		}
	}
	return 0;
}
