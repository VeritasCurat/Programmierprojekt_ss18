/*
 ============================================================================
 Name        : Programmierprojekt.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "adjazensliste.h"

char* eingabe;

void transformation();

void einlesen(){
	char buffer[10000];
	if(read(STDIN_FILENO, buffer, 10000) == 0){

	}
	eingabe = buffer;
}

int chartoint(char c){
	switch(c){
		case '0': return 0; break;
		case '1': return 1; break;
		case '2': return 2; break;
		case '3': return 3; break;
		case '4': return 4; break;
		case '5': return 5; break;
		case '6': return 6; break;
		case '7': return 7; break;
		case '8': return 8; break;
		case '9': return 9; break;
		default: return -1; break;
	}
}

void koordinaten_auslesen(char* zeile, int a, int b){
	a=b=0;
	int i=0;

	for(;i<strlen(zeile); i++){
		if(zeile[i]==' ')continue;
		if(chartoint(zeile[i]) != -1) break;
	}

	for(;i<strlen(zeile); i++){
		if(zeile[i]==' ')break;
		if(chartoint(zeile[i]) != -1){
			a*=10;
			a+=chartoint(zeile[i]);
		}
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')continue;
			if(chartoint(zeile[i]) != -1) break;
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')break;
			if(chartoint(zeile[i]) != -1){
				b*=10;
				b+=chartoint(zeile[i]);
			}
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')continue;
			if(chartoint(zeile[i]) != -1) break;
	}

}

void transformation(){
	for(int i=0; i<strlen(eingabe); i++){
		if(eingabe[i] == '\n'){
			int a,b;
			koordinaten_auslesen(eingabe, a, b);
			einfuegen(a,b);
		}
	}
}

/*
int basisalgo(adjazensliste problem, kachelliste l, adjazensliste loesung){}
	adjazensliste problem, kachelliste l, adjazensliste loesung, char* nachbar, kachel* akt) == -1)


}

int ausprobieren()


*/
/*
void test(){
	char *test ="0001213   133";
	int a,b;
	koordinaten_auslesen(test, a,b);
	printf("A: %d, B: %d",a,b);
}
*/

/*

int main(void) {

	//einlesen();
	//printf("%s\n",eingabe);



	return EXIT_SUCCESS;
}
*/
