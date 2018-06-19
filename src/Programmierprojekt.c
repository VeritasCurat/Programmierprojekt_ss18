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
	if(a > 4294967296 || b > 4294967296)exit(-1);
	//printf("A: %d B: %d\n", a,b);
	++anzKacheln; ++EL_anz;
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

int abs(int a){
	if(a < 0) return a*-1;
	return a;
}

int benachbart(int a[], int b[]){
	if(abs(b[0] - a[0]) + abs(b[1] - a[1]) == 1) return 1;
	return 0;
}

void test(){
char * test ="0 0\n"
			 "1 0\n"
			 "1 1\n"
			 "2 1";
/*
	char *test = "1 3\n"
			"5 2\n"
			"6 4\n"
			"5 6\n"
			"1 7\n"
			"5 3\n"
			"5 1\n"
			"4 5\n"
			"4 7\n"
			"5 7\n"
			"7 3\n"
			"4 4\n"
			"3 6\n"
			"1 4\n"
			"0 3\n"
			"7 0\n"
			"3 7\n"
			"2 5\n"
			"4 0\n"
			"6 7\n"
			"6 2\n"
			"7 6\n"
			"2 7\n"
			"7 2\n"
			"7 4\n"
			"6 1\n"
			"4 1\n"
			"5 5\n"
			"1 6\n"
			"2 4\n"
			"7 7\n"
			"3 2\n"
			"4 6\n"
			"2 0\n"
			"1 2\n"
			"2 6\n"
			"3 4\n"
			"6 0\n"
			"2 2\n"
			"0 2\n"
			"6 6\n"
			"0 6\n"
			"4 2\n"
			"0 7\n"
			"5 0\n"
			"3 1\n"
			"6 3\n"
			"1 0\n";
*/

	eingabe = test;
	//printf("%s\n",eingabe);


	transformation();

	//printlist();
}

void funktionstest1(){
	int a[] = {1,1};
	int b[] = {0,2};

	printf("benachbart(a,b): %d\n",benachbart(a,b));

}

//quadr. lösung
void raeume_prim(){
	//printf("liste: ");printlist();

	int index = 0;
	int gruppiert = 0;

	while(gruppiert < EL_anz){
		//schreibe erstes ELement (nicht (-1,-1) in neue Gruppe
			for(int k=0; k<EL_anz; k++){
				if(!(Element_liste[k][0] == -1 && Element_liste[k][1] == -1)){
					Raeume[index][0][0] = Element_liste[k][0];
					Raeume[index][0][1] = Element_liste[k][1];

				    ++R_anz[index];

				    Element_liste[k][0] =-1; Element_liste[k][1] =-1;
				    ++gruppiert;
				    break;
				}
			}

			printf("gruppe(%d): (%d,%d)\n", index,Raeume[index][0][0], Raeume[index][0][1]);

			int aenderungen=0;


				do{
					aenderungen=0;
					for(int i=0; i<R_anz[index]; i++){
						for(int j=0; j<anzKacheln; j++){
							//printf("bachbart?: (%d,%d)(%d,%d)\n", Raeume[index][i][0],Raeume[index][i][1], Element_liste[j][0], Element_liste[j][1]);
							if(benachbart(Raeume[index][i], Element_liste[j]) == 1){
								//printf("    bachbart: (%d,%d)(%d,%d)\n", Raeume[index][i][0],Raeume[index][i][1], Element_liste[j][0], Element_liste[j][1]);

								Raeume[index][R_anz[index]][0] = Element_liste[j][0];
								Raeume[index][R_anz[index]][1] = Element_liste[j][1];
								//printf("gruppe(%d): (%d,%d)\n", index, Raeume[index][R_anz[index]][0],  Raeume[index][R_anz[index]][1]);


								Element_liste[j][0] = -1; Element_liste[j][1]=-1;
								++aenderungen;
								++gruppiert;
								++R_anz[index];
							}
						}
					}

				}while(aenderungen > 0);


		//sortiere (notwendig?)
		//sort();
		++index;
	}

}

//quadr. lösung
void loesung_prim(int index){
	int p,q;
	for(int i=0; i<2^R_anz[100]; ++i){

	}

}

//tauscht solange indezes, bis alle benachbart sind: raueme_bubble





int main(void) {
	setbuf(stdout, NULL); //Printout bug lösen

	//einlesen();

	test();


	sort();
	//printlist();


	raeume_prim();
	printraeume();

	/*
	int index =0;
	while(R_anz[index] != 0){
			if(R_anz[index]%2==1){
				printf("In einem der Rauume ist die Anzahl der Plätze ungerade => unlösbar!");
				exit(0);
			}
			++index;
	}
	*/



	//funktionstest1();
	return EXIT_SUCCESS;
}
