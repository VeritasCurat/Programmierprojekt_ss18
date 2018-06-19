/*
 ============================================================================
 Name        : Programmierprojekt.c
 Author      : Johannes Grobelski
 Version     :
 Copyright   : Your copyright notice
 Description : Programmierprojekt SS18
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
	char buffer[100000];
	if(read(STDIN_FILENO, buffer, 100000) == 0){

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

void koordinate_einlesen(char* zeile){
	int a,b; a=b=0;

	//printf("Zeile: %s\n",zeile);

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
	//printf("A: %d B: %d\n", a,b);
	++anzKacheln;
	einfuegen(a,b);
}

void transformation(){
	int anfang, ende =0;
	for(int i=0; i<=strlen(eingabe); i++){

		if(eingabe[i-1] == '\n' || i == strlen(eingabe)){
			char segment[ende-anfang];
			for(int i=0; i<ende-anfang+1; i++){
				segment[i] = eingabe[i+anfang];
			}
			segment[ende-anfang+1] = '\0';


			//printf("Segment: %s\n",segment);
			koordinate_einlesen(segment);


			anfang = ende+1;
		}
		ende = i;
	}
	if(anzKacheln % 2 == 1){
		printf("Lösung existiert nicht! UNgerade Anzahl an Kacheln (%d)", anzKacheln);
		exit(0);
	}
}

/*
int basisalgo(adjazensliste problem, kachelliste l, adjazensliste loesung){}
	adjazensliste problem, kachelliste l, adjazensliste loesung, char* nachbar, kachel* akt) == -1)


}

int ausprobieren()


*/

void test(){
	char *test = "0 0 \n"
            	 "0 1\n"
				 "0 3\n"
				 "1 0 \n"
				 "1 2\n"
				 "1 3\n";


	eingabe = test;
	//printf("%s\n",eingabe);


	transformation();

	//printlist();
}

void raeume(){
	int breite, hoehe, index = 0;

	/*
	while(index < position){
		if(){
			//nachbar hinzufuegen
		}
		else if(){//sprung in x)
				if(){
					//nachbar hinzufegen
				}
				else{
					//raum beendet
				}
		}
		++index;
	}
	*/

}


int main(void) {
	setbuf(stdout, NULL); //Printout bug lösen

	einlesen();
	//test();


	sort();
	printlist();


	return EXIT_SUCCESS;
}
