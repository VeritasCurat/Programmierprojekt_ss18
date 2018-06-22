/*
 ============================================================================
 Name        : Programmierprojekt.c
 Author      : Johannes Grobelski
 Version     :
 Copyright   : Your copyright noticeo,,,
 Description : Programmierprojekt SS18
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char strategie = 'b'; //s=sortieren, h=hashing, b=beides




//Hashing
	int maxNEXT = 10;
	int H_belegt = 0;
	int H_gruppiert = 0;
	int H_geloest= 0;

	int H_P = 999983;

	unsigned int Hash_liste[999983][2]; //TODO: variabel machen (anzKacheln)
	unsigned int Hash_gruppiert[999983][2];
	unsigned int Hash_geloest[999983][2];

	int H_seed = 777;

	unsigned int Element_listeR[999983][2];
	unsigned int Loesung_R[999983][4]; 	int index_loesung=0;


//Adjazensliste + sort
	unsigned int Element_liste[100000][2]; int EL_anz;
	unsigned int Loesung_liste[100000][4];
	unsigned int Raeume[100][100000][2]; int R__El_anz[100];
	unsigned int Loesung_Raeume[100][100000][4]; int R__El_anz[100];


	unsigned int anzKacheln;
	int position=0;
	size_t liste_size = 10000;

	void transformation();


int H_HP_berechnen(unsigned int x,unsigned int y){
	//printf("HS berechnen: (%u,%u)\n", x,y);

	int k =  (1000*x + 10*y) % H_P;
//	k^=323522*(x*y);
	return k;
}

int H_next_berechnen(unsigned int k, unsigned int next){
	k =  (k+next) % H_P;
//	k^=323522*(x*y);
	return k;
}


//HS: (x,y) -> 1000*x + 10*y % P
void H_init(){
	for(int i=0; i<H_P; i++){
		Hash_liste[i][0] = -1; Hash_liste[i][1] = -1;
		Hash_gruppiert[i][0] = -1; Hash_gruppiert[i][1] = -1;
		Hash_geloest[i][0] = -1; Hash_geloest[i][1] = -1;
	}


}


void H_gruppieren(unsigned int x,unsigned int y){
	//printf("		grupiere: (%d,%d)\n:",x,y);
	if(H_gruppiert == H_P){
		printf("Hashtabelle voll!");
		exit(-1);
	}
	unsigned int k =H_HP_berechnen(x,y);
	if(Hash_gruppiert[k][0] == -1 && Hash_gruppiert[k][1] ==-1){
		Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
		//printf("ASH: %d,%d -> %d\n",x,y,k);
	}
	else {
		//inplace hashing
		unsigned int next=2;
		while(!(Hash_geloest[H_next_berechnen(k,next)][0] == -1 && Hash_geloest[H_next_berechnen(k,next)][1] ==-1)){
			printf("						Hashing Kollision: (%d,%d)(%d,%d)\n", x,y,Hash_geloest[k*next*next % H_P][0],Hash_geloest[k*next*next % H_P][1]);
			printf("						Hashing Kollision Data: (%d,%d)\n", k,next);
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("B Hashtabelle zu dicht belegt! (%d)",H_gruppiert);
				exit(-1);
			}
			++next;
		}
		Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
	}
}

void H_loesen(unsigned int x, unsigned int y){
//	printf("		H_loesen eintragen: (%u,%u)\n:",x,y);
	if(H_geloest == H_P){
		printf("Hashtabelle voll!");
		exit(-1);
	}
	unsigned int k =H_HP_berechnen(x,y);
	if(Hash_geloest[k][0] == -1 && Hash_geloest[k][1] ==-1){
		Hash_geloest[k][0] = x; Hash_geloest[k][1] = y;
		//printf("ASH: %d,%d -> %d\n",x,y,k);
	}
	else {
		printf("Hashing Try: (%u,%u)\n",x,y );
		//inplace hashing
		unsigned int next=2;
		while(!(Hash_geloest[H_next_berechnen(k,next)][0] == -1 && Hash_geloest[H_next_berechnen(k,next)][1] ==-1)){
			printf("						Hashing Kollision: (%u,%u)(%u,%u)\n", x,y,Hash_geloest[k*next*next % H_P][0],Hash_geloest[k*next*next % H_P][1]);
			printf("						Hashing Kollision Data: (%u,%u)\n", k,next);

			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("C Hashtabelle zu dicht belegt! ()");
				exit(-1);
			}
			++next;
		}
		Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
	}
}

void H_loesen_loeschen(unsigned int x, unsigned int y){
	//printf("				suche: (%d,%d)\n",x,y);
	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_liste[k][0] == x && Hash_liste[k][1] ==y){
		Hash_liste[k][0] = -1; Hash_liste[k][1] = -1;
	}
	else {
		//inplace hashing
		unsigned int next=2;
		while(!(Hash_geloest[H_next_berechnen(k,next)][0] == -1 && Hash_geloest[H_next_berechnen(k,next)][1] ==-1)){
			printf("						Hashing Kollision: (%u,%u)(%u,%u)\n", x,y,Hash_geloest[k*next*next % H_P][0],Hash_geloest[k*next*next % H_P][1]);
			printf("						Hashing Kollision Data: (%u,%u)\n", k,next);
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("A Hashtabelle zu dicht belegt!");
				exit(-1);
			}
			++next;
		}
		Hash_gruppiert[k][0] = -1; Hash_gruppiert[k][1] = -1;

	}
}

void H_eintragen(unsigned int x, unsigned int y){
	if(H_belegt == H_P){
		printf("Hashtabelle voll!");
		exit(-1);
	}
	unsigned int k = H_HP_berechnen(x,y);
	//printf("HS: k: %d\n",k);

	if(Hash_liste[k][0] == -1 && Hash_liste[k][1] ==-1){
		//printf("HS eintragen V1: (%u,%u)\n", x,y);
		Hash_liste[k][0] = x; Hash_liste[k][1] = y;
		//printf("ASH: %d,%d -> %d\n",x,y,k);
	}
	else {
		//printf("HS eintragen V2: (%u,%u) wegen (%u,%u)\n", x,y,Hash_liste[k][0],Hash_liste[k][1]);

		//inplace hashing
		unsigned int next=2;
		while(!(Hash_geloest[H_next_berechnen(k,next)][0] == -1 && Hash_geloest[H_next_berechnen(k,next)][1] ==-1)){
			printf("						Hashing Kollision: (%u,%u)(%u,%u)\n", x,y,Hash_geloest[k*next*next % H_P][0],Hash_geloest[k*next*next % H_P][1]);
			printf("						Hashing Kollision Data: (%u,%u)\n", k,next);
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("A Hashtabelle zu dicht belegt!");
				exit(-1);
			}
			++next;
		}
		Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
	}
}

void H_hashtabelle_erstellen(){
	for(int index=0; index<anzKacheln; ++index){
		 // printf("HS eintragen: (%u,%u))\n",Element_liste[index][0], Element_liste[index][1] );
			H_eintragen(Element_liste[index][0], Element_liste[index][1]);
	}
}

int H_suchen(unsigned int x, unsigned int y){
	//printf("				suche: (%d,%d)\n",x,y);
	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_liste[k][0] == x && Hash_liste[k][1] ==y){
		return k;
	}
	else if(Hash_liste[k][0] == -1 && Hash_liste[k][1] == -1){
			return -1;
	}
	else {
		//inplace hashing
		unsigned int next=2;
		while(1){
			k = H_next_berechnen(k,x);
			if(Hash_liste[k][0] == x && Hash_liste[k][1] == y) return k;
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
		return k*next*next % H_P;
	}
	return -1;
}

int H_abfrage_gruppiert(unsigned int x, unsigned int y){
	//printf("		grupiert?: (%d,%d)\n:",x,y);

	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_gruppiert[k][0] == x && Hash_gruppiert[k][1] ==y){
		return k;
	}
	else if(Hash_gruppiert[k][0] == -1 && Hash_gruppiert[k][1] == -1){
			return -1;
	}
	else {
		//inplace hashing
		unsigned int next=2;
		while(1){
			k = H_next_berechnen(k,x);
			if(Hash_gruppiert[k][0] == x && Hash_gruppiert[k][1] == y) return k;
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
		return k*next*next % H_P;
	}
	return -1;
}

int H_abfrage_geloest(unsigned int x, unsigned int y){
	//printf("		grupiert?: (%d,%d)\n:",x,y);

	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_geloest[k][0] == x && Hash_geloest[k][1] ==y){
		return k;
	}
	else if(Hash_geloest[k][0] == -1 && Hash_geloest[k][1] == -1){
			return -1;
	}
	else {
		//inplace hashing
		unsigned int next=2;
		while(1){
			k = H_next_berechnen(k,x);
			if(Hash_geloest[k][0] == x && Hash_geloest[k][1] == y) return k;
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
		return k*next*next % H_P;
	}
	return -1;
}

void H_printHash(){
	for(int index=0; index<anzKacheln; ++index){
		int k = H_suchen(Element_liste[index][0], Element_liste[index][1]);
		printf("Platz %u: (%u,%u)\n", k, Hash_liste[k][0], Hash_liste[k][1]);
	}
}

void init(){

//	int (*Element_liste)[2] = malloc(sizeof(int[liste_size][2])); //TODOfree(Element_liste)

	for(int i=0; i<liste_size; i++){
		Element_liste[i][0] = -1; Element_liste[i][1] = -1;
	}

	for(int raum=0; raum<100; raum++){
			for(int i=0; i<liste_size; i++){
						Raeume[raum][i][0] = -1; Raeume[raum][i][1] = -1;
			}
	}


}

void printlist(){
	for(int i=0; i<position; i++){
				printf("%u: A: %u, B: %u\n", i, Element_liste[i][0] , Element_liste[i][1]);
	}
}

void printraeume(){
	int index =0;
	while(R__El_anz[index] != 0){
		printf("Raum %d (%d):\n",index,R__El_anz[index]);
		for(int i=0; i<R__El_anz[index]; i++){
			printf("x: %u, y: %u\n", Raeume[index][i][0], Raeume[index][i][1]);
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
void listevergroessern(){ /TODO: malloc
	liste_size *= 2;
	Element_liste = (int **)  realloc(Element_liste, (liste_size * 2) * sizeof(int)*2);
}
*/

void einfuegen(unsigned int x,unsigned int y){
	//printf("x: %u y: %u\n", x,y);

	/*
	if(position == liste_size){ //TODO: liste vergroessern
		listevergroessern();
	}
	*/

	//printlist();

	Element_liste[position][0] = (unsigned int) x;
	Element_liste[position][1] = (unsigned int) y;
  //printf("eingefuegt %u,%u\n", Element_liste[position][0],Element_liste[position][1]);

	++position;
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

unsigned int chartoint(char c){
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
	unsigned int a,b; a=b=0;

	int a_len = 0; int b_len =0;

	//printf("Zeile: %s\n",zeile);

	int i=0;

	for(;i<strlen(zeile); i++){
		if(zeile[i]==' ')continue;
		if(chartoint(zeile[i]) != -1) break;
	}


	for(;i<strlen(zeile); i++){
		if(zeile[i]==' ')break;
		else if(chartoint(zeile[i]) != -1){
			if(a>0)++a_len;
			a*=(unsigned) 10;
			a+=(unsigned) chartoint(zeile[i]);
		}
		else {
			printf("illegales Zeichen! (%c)\n",zeile[i]);
			exit(-1);
		}
		if(a_len > 10){
			printf("%s (%s)\n", "zu lange Zahlen!",zeile);
			exit(-1);
		}
	}


	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')continue;
			if(chartoint(zeile[i]) != -1) break;
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ' || zeile[i]=='\n')break;
			else if(chartoint(zeile[i]) != -1){
				if(b>0)++b_len;
				b*=(unsigned) 10;
				b+=(unsigned) chartoint(zeile[i]);
			}
			else {
				printf("illegales Zeichen! (%c)\n",zeile[i]);
				exit(-1);
			}
			if(b_len > 10){
				printf("%s (%s)\n", "zu lange Zahlen! ",zeile);
				exit(-1);
			}
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')continue;
			if(chartoint(zeile[i]) != -1) break;
	}
	if(a >= 4294967296 || b >= 4294967296 || a < 0 || b < 0){
		printf("%s\n", "zu lange Zahlen!");
		exit(-1);
	}
		 //4294967291
	//printf("A: %u B: %u\n", a,b);
	++anzKacheln; ++EL_anz;
	einfuegen(a,b);
}

void einlesen(){
	char buffer[1000000];


	while(fgets(buffer, sizeof(buffer), stdin) != NULL){
		koordinate_einlesen(buffer);
	}


}




void transformation(char* eingabe){
	//printf("Eingabe: %s\n",eingabe);
	int anfang = 0; int ende =0;
	for(int i=0; i<strlen(eingabe); i++){

		if(eingabe[i] == '\n' || i == strlen(eingabe)){
			ende = i;

			char segment[ende-anfang];
			for(int i=0; i<ende-anfang; i++){
				segment[i] = eingabe[i+anfang];
			}
			//segment[ende-anfang+1] = '\0';

			//printf("Segment: %s\n",segment);
			koordinate_einlesen(segment);


			anfang = ende+1;
		}
		ende = i;
	}
	if(anzKacheln % 2 == 1){
		printf("Loesung existiert nicht! UNgerade Anzahl an Kacheln (%d)", anzKacheln);
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


unsigned int benachbart(unsigned int a[2], unsigned int b[2]){
	int k= abs(b[0] - a[0]) + abs(b[1] - a[1]) == 1;
	if(k == 1) return 1;
	return 0;
}




void test(){
/*
char * test ="1 1\n"
			 "2 1\n"
			 "2 2\n"
			 "3 2\n";
*/


char * test = "0 0\n"
			 		  	"0 1\n"
			 				"0 3\n"
			 				"1 0\n"
			 				"1 2\n"
			 				"1 3\n";




	//printf("%s\n",eingabe);

	transformation(test);
	//printlist();
}

void funktionstest1(){
 unsigned	int a[] = {1,1};
 unsigned int b[] = {0,2};

	printf("benachbart(a,b): %d\n",benachbart(a,b));
}

void funktionstestH(){



}

//n*|raeuem| loesung
void raeume_prim(){
	if(anzKacheln % 2 == 1){
		printf("Keine Loesung moeglich (ungerade Anzahl Kacheln!)\n");
		exit(0);
	}
	//printf("liste: ");printlist();

	unsigned int index = 0;
	int gruppiert = 0;

	while(gruppiert < EL_anz){
		//schreibe erstes ELement (nicht (-1,-1) in neue Gruppe
			for(int k=0; k<EL_anz; k++){
				if(!(Element_liste[k][0] == -1 && Element_liste[k][1] == -1)){
					Raeume[index][0][0] = Element_liste[k][0];
					Raeume[index][0][1] = Element_liste[k][1];

				    ++R__El_anz[index];

				    Element_liste[k][0] =-1; Element_liste[k][1] =-1;
				    ++gruppiert;
				    break;
				}
			}

			printf("gruppe(%d): (%u,%u)\n", index,Raeume[index][0][0], Raeume[index][0][1]);

			int aenderungen=0;


				do{
					aenderungen=0;
					for(unsigned int i=0; i<R__El_anz[index]; i++){
						for(unsigned int j=0; j<anzKacheln; j++){
							//printf("bachbart?: (%d,%d)(%d,%d)\n", Raeume[index][i][0],Raeume[index][i][1], Element_liste[j][0], Element_liste[j][1]);
							if(benachbart(Raeume[index][i], Element_liste[j]) == 1){
								//printf("    bachbart: (%d,%d)(%d,%d)\n", Raeume[index][i][0],Raeume[index][i][1], Element_liste[j][0], Element_liste[j][1]);

								Raeume[index][R__El_anz[index]][0] = Element_liste[j][0];
								Raeume[index][R__El_anz[index]][1] = Element_liste[j][1];
								//printf("gruppe(%d): (%d,%d)\n", index, Raeume[index][R__El_anz[index]][0],  Raeume[index][R__El_anz[index]][1]);


								Element_liste[j][0] = -1; Element_liste[j][1]=-1;
								++aenderungen;
								++gruppiert;
								++R__El_anz[index];
							}
						}
					}

				}while(aenderungen > 0);


		//sortiere (notwendig?)
		//sort();
		++index;
	}

}


void raeume_linearH(){
	if(anzKacheln % 2 == 1){
		printf("Keine Loesung moeglich (ungerade Anzahl Kacheln!)\n");
		exit(0);
	}

	int index = 0;
		int gruppiert = 0;

		while(gruppiert < EL_anz){
			//schreibe erstes ELement (nicht (-1,-1) in neue Gruppe
				for(int k=0; k<EL_anz; k++){
					if(!(Element_liste[k][0] == -1 && Element_liste[k][1] == -1)){
						if(H_abfrage_gruppiert(Element_liste[k][0],Element_liste[k][1]) > -1)continue;
						Raeume[index][0][0] = Element_liste[k][0];
						Raeume[index][0][1] = Element_liste[k][1];
					    ++R__El_anz[index];

						H_gruppieren(Element_liste[k][0],Element_liste[k][1]);
					//	printf("gruppiert / EL_anz: %u %u\n",gruppiert,EL_anz);
						printf("gruppe(%d): repraesentant: (%u,%u), gruppiert: %d\n", index,Raeume[index][0][0], Raeume[index][0][1],gruppiert);


					    Element_liste[k][0] =-1; Element_liste[k][1] =-1;
					    ++gruppiert;
					    break;
					}
				}


				int i=0;
				int k=0;
				while(i < R__El_anz[index]){
					//if(gruppiert % 100 <5)printf("raeume: %d / %d",gruppiert,anzKacheln);

					//printf("		Betrachte: (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1]);
					//linker Nachbar?
						k = H_suchen(Raeume[index][i][0],Raeume[index][i][1]-1);
						if(k!=-1){
							//printf("L	Nachbarn: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);
							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("		Nachbarn eintragen: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R__El_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R__El_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R__El_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}
					//rechter Nachbar?
						k = H_suchen(Raeume[index][i][0],Raeume[index][i][1]+1);
						if(k!=-1){
							//printf("R	Nachbarn: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("R	Nachbarn eintragen: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R__El_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R__El_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R__El_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}
					//oberer Nachbar?
						k = H_suchen(Raeume[index][i][0]+1,Raeume[index][i][1]);
						if(k!=-1){
						//	printf("O	Nachbarn: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("O	Nachbarn eintragen: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R__El_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R__El_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R__El_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}
					//unterer Nachbar?
						k = H_suchen(Raeume[index][i][0]-1,Raeume[index][i][1]);
						if(k!=-1){
							//printf("U	Nachbarn: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("U	Nachbarn eintragen: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R__El_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R__El_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R__El_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}

						//printf("		i=%d / Anzahl Element(Raum)=%d\n",i,R__El_anz[index]);

						++i;
				}
			if(R__El_anz[index] % 2 == 1){
				printf("Ein Raum hat eine ungerade Anzahl von Kacheln (Raum %d: %d Kacheln)\n",index, R__El_anz[index] );
			}
			//sortiere (notwendig?)
			//sort();
			++index;
		}

}

void greedy(){

}

int validate(){
	for(int i=0; i<500000; i++){
		if(Loesung_R[i][0] == -1 &&	Loesung_R[i][1] == -1 && Loesung_R[i][2] == -1 &&	Loesung_R[i][3] == -1)break;
		if(abs(Loesung_R[i][0] - Loesung_R[i][2])  +	abs(Loesung_R[i][1] -	Loesung_R[i][3]) != 1){
			printf("FALSCH: %d %d;%d %d\n", Loesung_R[i][0],Loesung_R[i][1],Loesung_R[i][2],Loesung_R[i][3]);
			return 0;
		}
	}
	return 1;
}

int loesung_prim(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung){
	if(index_loesung_raum== R__El_anz[raum]/2){
		printf("loesung errechnet, %d\n", index_loesung);

		return 1;

	} //gueltige loesung

	/*
	1. wiederhole, solange es noch neue Teil-Lösungsschritte gibt:
	     a) wähle einen neuen Teil-Lösungsschritt;
	     b) falls Wahl gültig ist:
	               I) erweitere Vektor um Wahl;
	              II) falls Vektor vollständig ist, return true; // Lösung gefunden!
	                  sonst:
	                       falls (FindeLoesung(Stufe+1, Vektor)) return true; // Lösung!
	                       sonst mache Wahl rückgängig; // Sackgasse (Backtracking)!
	  2. Da es keinen neuen Teil-Lösungsschritt gibt: return false // Keine Lösung!


 	*/

		int a[2];
		int nachbar[2];
		int suche=0;
		int geloest=0;

		int l=0;

		//1. nehme erste verbleibende in Element_liste
		int i;
			for(i=0; i<R__El_anz[0]; i++){
				if(Raeume[raum][i][0] ==-1 && Raeume[raum][i][1] == -1)continue;
				//printf("%u,%u (gruppiert:%d)\n", Raeume[0][i][0], Raeume[0][i][1],index_loesung);

				//pruefe ob kachel aus Element_liste schon in loesung
				geloest = H_abfrage_geloest(Raeume[raum][i][0], Raeume[raum][i][1]);
				if(geloest == -1){
					a[0] = Raeume[raum][i][0];
					a[1] = Raeume[raum][i][1];
					Raeume[raum][i][0] = -1;
					Raeume[raum][i][1] = -1;
					break;
				}
			}

		//2. suche Nachbarn
			//rechter Nachbarn?
			suche = H_suchen(a[0], a[1]+1);
			if(suche > 0){

								//TODO: Funktion
								//pruefe ob kachel aus Element_liste schon in loesung
								geloest = H_abfrage_geloest(Hash_liste[suche][0],Hash_liste[suche][1]);
								if(geloest == -1){
									//printf("		Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",a[0],a[1],Hash_liste[suche][0],Hash_liste[suche][1],index_loesung);

									H_loesen(a[0],a[1]);
									H_loesen(Hash_liste[suche][0],Hash_liste[suche][1]); //TODO: mgl. Raeume[?][i][0,1] herrausfinden

									nachbar[0] = 	Hash_liste[suche][0];
									nachbar[1] =  Hash_liste[suche][1];



									loesung[index_loesung][0] = a[0];
									loesung[index_loesung][1] = a[1];
									loesung[index_loesung][2] = nachbar[0];
									loesung[index_loesung][3] = nachbar[1];

									Hash_liste[suche][1] =-1;
									Hash_liste[suche][1] =-1;
									++index_loesung;
									++index_loesung_raum;

									l = loesung_prim(raum, loesung, index_loesung_raum,index_loesung);

									if(l == 1) return 1;
									else{
										H_loesen_loeschen(a[0],a[1]);
										H_loesen_loeschen(nachbar[0],nachbar[1]);
										//teilloesung nicht valide => rollback
										loesung[index_loesung][0] = -1;
										loesung[index_loesung][1] = -1;
										loesung[index_loesung][2] = -1;
										loesung[index_loesung][3] = -1;

										Raeume[raum][i][0] = a[0];
										Raeume[raum][i][1] = a[1];

										--index_loesung;
										--index_loesung_raum;
									}
								}
			}



			//linker Nachbarn?
			suche = H_suchen(a[0], a[1]-1);
			if(suche > 0){

								//TODO: Funktion
								//pruefe ob kachel aus Element_liste schon in loesung
								geloest = H_abfrage_geloest(Hash_liste[suche][0],Hash_liste[suche][1]);
								if(geloest == -1){
									//printf("		Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",a[0],a[1],Hash_liste[suche][0],Hash_liste[suche][1],index_loesung);

									H_loesen(a[0],a[1]);
									H_loesen(Hash_liste[suche][0],Hash_liste[suche][1]); //TODO: mgl. Raeume[?][i][0,1] herrausfinden

									nachbar[0] = 	Hash_liste[suche][0];
									nachbar[1] =  Hash_liste[suche][1];



									loesung[index_loesung][0] = a[0];
									loesung[index_loesung][1] = a[1];
									loesung[index_loesung][2] = nachbar[0];
									loesung[index_loesung][3] = nachbar[1];

									Hash_liste[suche][1] =-1;
									Hash_liste[suche][1] =-1;
									++index_loesung;
									++index_loesung_raum;

									l = loesung_prim(raum, loesung,index_loesung_raum, index_loesung);

									if(l == 1) return 1;
									else{
										H_loesen_loeschen(a[0],a[1]);
										H_loesen_loeschen(nachbar[0],nachbar[1]);
										//teilloesung nicht valide => rollback
										loesung[index_loesung][0] = -1;
										loesung[index_loesung][1] = -1;
										loesung[index_loesung][2] = -1;
										loesung[index_loesung][3] = -1;

										Raeume[raum][i][0] = a[0];
										Raeume[raum][i][1] = a[1];

										--index_loesung;
										--index_loesung_raum;
									}
								}
			}

			//oberer Nachbarn?
			suche = H_suchen(a[0]+1, a[1]);
			if(suche > 0){

								//TODO: Funktion
								//pruefe ob kachel aus Element_liste schon in loesung
								geloest = H_abfrage_geloest(Hash_liste[suche][0],Hash_liste[suche][1]);
								if(geloest == -1){
									//printf("		Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",a[0],a[1],Hash_liste[suche][0],Hash_liste[suche][1],index_loesung);

									H_loesen(a[0],a[1]);
									H_loesen(Hash_liste[suche][0],Hash_liste[suche][1]); //TODO: mgl. Raeume[?][i][0,1] herrausfinden

									nachbar[0] = 	Hash_liste[suche][0];
									nachbar[1] =  Hash_liste[suche][1];



									loesung[index_loesung][0] = a[0];
									loesung[index_loesung][1] = a[1];
									loesung[index_loesung][2] = nachbar[0];
									loesung[index_loesung][3] = nachbar[1];

									Hash_liste[suche][1] =-1;
									Hash_liste[suche][1] =-1;
									++index_loesung;
									++index_loesung_raum;

									l = loesung_prim(raum, loesung, index_loesung_raum,index_loesung);

									if(l == 1) return 1;
									else{
										H_loesen_loeschen(a[0],a[1]);
										H_loesen_loeschen(nachbar[0],nachbar[1]);
										//teilloesung nicht valide => rollback
										loesung[index_loesung][0] = -1;
										loesung[index_loesung][1] = -1;
										loesung[index_loesung][2] = -1;
										loesung[index_loesung][3] = -1;

										Raeume[raum][i][0] = a[0];
										Raeume[raum][i][1] = a[1];

										--index_loesung;
										--index_loesung_raum;
									}
								}
			}

			//unterer Nachbarn?
			suche = H_suchen(a[0]-1, a[1]);
			if(suche > 0){

				//TODO: Funktion
				//pruefe ob kachel aus Element_liste schon in loesung
				geloest = H_abfrage_geloest(Hash_liste[suche][0],Hash_liste[suche][1]);
				if(geloest == -1){
					//printf("		Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",a[0],a[1],Hash_liste[suche][0],Hash_liste[suche][1],index_loesung);

					H_loesen(a[0],a[1]);
					H_loesen(Hash_liste[suche][0],Hash_liste[suche][1]); //TODO: mgl. Raeume[?][i][0,1] herrausfinden

					nachbar[0] = 	Hash_liste[suche][0];
					nachbar[1] =  Hash_liste[suche][1];



					loesung[index_loesung][0] = a[0];
					loesung[index_loesung][1] = a[1];
					loesung[index_loesung][2] = nachbar[0];
					loesung[index_loesung][3] = nachbar[1];

					Hash_liste[suche][1] =-1;
					Hash_liste[suche][1] =-1;
					++index_loesung;
					++index_loesung_raum;

					l = loesung_prim(raum, loesung, index_loesung_raum,index_loesung);

					if(l == 1) return 1;
					else{
						H_loesen_loeschen(a[0],a[1]);
						H_loesen_loeschen(nachbar[0],nachbar[1]);
						//teilloesung nicht valide => rollback
						loesung[index_loesung][0] = -1;
						loesung[index_loesung][1] = -1;
						loesung[index_loesung][2] = -1;
						loesung[index_loesung][3] = -1;

						Raeume[raum][i][0] = a[0];
						Raeume[raum][i][1] = a[1];

						--index_loesung;
						--index_loesung_raum;
					}
				}
			}

		//	printf("%s\n","exiting..." );
		//	exit(0);
			return 0;
}


int main(void) {
//TODO: raueme_lin macht bei ex1 aus (0,0) -> (2,0)


	//setbuf(stdout, NULL); //Printout bug lösen
	init();
	H_init();

	//funktionstestH();exit(0);

	einlesen(); //Dauert bei ex4 schon 60s

	//test();

	//printf("eingelesen ... !");

//printf("transformiert ... !");
	//printlist();


	H_hashtabelle_erstellen();
printf("hs angefertigt ... !");


	//sort();

	//H_printHash();

	raeume_linearH();
	//raeume_prim();

	printraeume();


	/*
	int index =0;
	while(R__El_anz[index] != 0){
			if(R__El_anz[index]%2==1){
				printf("In einem der Rauume ist die Anzahl der Plätze ungerade => unlösbar!");
				exit(0);
			}
			++index;
	}
	*/

	for(int i=0; i<999983; i++){
		Loesung_R[i][0] = -1;
		Loesung_R[i][1] = -1;
		Loesung_R[i][2] = -1;
		Loesung_R[i][3] = -1;
	}

	for(int raum = 0; ;++raum){
		if(R__El_anz[raum] == 0)break;
		loesung_prim(raum, Loesung_R, 0,index_loesung);
		printf("index_loesung: %d\n",index_loesung );
	}

  printf("\n\nLoesung:\n");
	for(int i=0; i<index_loesung; i++){
		if(Loesung_R[i][0] == -1 &&	Loesung_R[i][1] == -1 && Loesung_R[i][2] == -1 &&	Loesung_R[i][3] == -1)continue;
		printf("%u %u;%u %u\n", Loesung_R[i][0],Loesung_R[i][1],Loesung_R[i][2],Loesung_R[i][3]);
	}

	printf("index_loesung: %u\n",index_loesung);

	printf("validate =%d\n",validate());
	printf("anzKacheln: %d\n",anzKacheln );

	exit(0);
	//funktionstest1();
	return EXIT_SUCCESS;
}
