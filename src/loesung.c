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

char strategie = 'b'; //s=sortieren, h=hashing, b=beides

char eingabe[1000000000];



//Hashing
	int maxNEXT = 10;
	int H_belegt = 0;
	int H_gruppiert = 0;
	int H_P = 999983;

	unsigned int Hash_liste[999983][2]; //TODO: variabel machen (anzKacheln)
	unsigned int Hash_gruppiert[999983][2];
	int H_seed = 777;

//Adjazensliste + sort
	unsigned int Element_liste[100000][2]; int EL_anz;
	unsigned int Loesung_liste[100000][4];
	unsigned int Raeume[100][100000][2]; int R_anz[100];
	unsigned int anzKacheln;
	int position=0;
	size_t liste_size = 10000;

	void transformation();


void H_HP_berechnen(){ //TODO

}

//HS: (x,y) -> 1000*x + 10*y % P
void H_init(){
	for(int i=0; i<H_P; i++){
		Hash_liste[i][0] = -1; Hash_liste[i][1] = -1;
		Hash_gruppiert[i][0] = -1; Hash_gruppiert[i][1] = -1;
	}


}


void H_gruppieren(int x, int y){
	//printf("		grupiere: (%d,%d)\n:",x,y);
	if(H_gruppiert == H_P){
		printf("Hashtabelle voll!");
		exit(-1);
	}
	int k =1000*x + 10*y % H_P;
	if(Hash_gruppiert[k][0] == -1 && Hash_gruppiert[k][1] ==-1){
		Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
		//printf("ASH: %d,%d -> %d\n",x,y,k);
	}
	else {
		//inplace hashing
		int next=2;
		while(Hash_gruppiert[k*next*next % H_P][0] == -1 && Hash_gruppiert[k*next*next % H_P][1] ==-1){
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("Hashtabelle zu dicht belegt!");
				exit(-1);
			}
			++next;
		}
		Hash_gruppiert[k*next*next % H_P][0] = x; Hash_gruppiert[k*next*next % H_P][1] = y;
	}
}

void H_eintragen(int x, int y){
	if(H_belegt == H_P){
		printf("Hashtabelle voll!");
		exit(-1);
	}
	int k =1000*x + 10*y % H_P;
	if(Hash_liste[k][0] == -1 && Hash_liste[k][1] ==-1){
		Hash_liste[k][0] = x; Hash_liste[k][1] = y;
		//printf("ASH: %d,%d -> %d\n",x,y,k);
	}
	else {
		//inplace hashing
		int next=2;
		while(Hash_liste[k*next*next % H_P][0] == -1 && Hash_liste[k*next*next % H_P][1] ==-1){
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				printf("Hashtabelle zu dicht belegt!");
				exit(-1);
			}
			++next;
		}
		Hash_liste[k*next*next % H_P][0] = x; Hash_liste[k*next*next % H_P][1] = y;
	}
}

void H_hashtabelle_erstellen(){
	for(int index=0; index<anzKacheln; ++index){
			H_eintragen(Element_liste[index][0], Element_liste[index][1]);
	}
}

int H_suchen(int x, int y){
	//printf("				suche: (%d,%d)\n",x,y);
	int k =1000*x + 10*y %H_P;
	if(Hash_liste[k][0] == x && Hash_liste[k][1] ==y){
		return k;
	}
	else if(Hash_liste[k][0] == -1 && Hash_liste[k][1] == -1){
			return -1;
	}
	else {
		//inplace hashing
		int next=2;
		while(Hash_liste[k*next*next % H_P][0] != x && Hash_liste[k*next*next % H_P][1] != y){
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
	}
	return -1;
}

int H_abfrage_gruppiert(int x, int y){
	//printf("		grupiert?: (%d,%d)\n:",x,y);

	int k =1000*x + 10*y %H_P;
	if(Hash_gruppiert[k][0] == x && Hash_gruppiert[k][1] ==y){
		return k;
	}
	else if(Hash_gruppiert[k][0] == -1 && Hash_gruppiert[k][1] == -1){
			return -1;
	}
	else {
		//inplace hashing
		int next=2;
		while(Hash_gruppiert[k*next*next % H_P][0] != x && Hash_gruppiert[k*next*next % H_P][1] != y){
			if(next == maxNEXT){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
	}
	return -1;
}

void H_printHash(){
	for(int index=0; index<anzKacheln; ++index){
		int k = H_suchen(Element_liste[index][0], Element_liste[index][1]);
		printf("Platz %d: (%d,%d)\n", k, Hash_liste[k][0], Hash_liste[k][1]);
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
void listevergroessern(){ /TODO: malloc
	liste_size *= 2;
	Element_liste = (int **)  realloc(Element_liste, (liste_size * 2) * sizeof(int)*2);
}
*/

void einfuegen(int x, int y){

	/*
	if(position == liste_size){ //TODO: liste vergroessern
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


void einlesen(){
	char buffer[1000000];
	while(fgets(buffer, sizeof(buffer), stdin) != NULL){
		strcat(eingabe, buffer);
		strcat(eingabe, "      ");

	}


}

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
	strcat(zeile, "");

	printf("Zeile: %s\n",zeile);

	int i=0;

	for(;i<strlen(zeile); i++){
		if(zeile[i]==' ')continue;
		if(chartoint(zeile[i]) != -1) break;
	}

	for(;i<strlen(zeile); i++){
		if(zeile[i]==' ')break;
		if(chartoint(zeile[i]) != -1){
			a*=(unsigned) 10;
			a+=(unsigned) chartoint(zeile[i]);
		}
	}


	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')continue;
			if(chartoint(zeile[i]) != -1) break;
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')break;
			if(chartoint(zeile[i]) != -1){
				b*=(unsigned) 10;
				b+=(unsigned) chartoint(zeile[i]);
			}
	}

	for(;i<strlen(zeile); i++){
			if(zeile[i]==' ')continue;
			if(chartoint(zeile[i]) != -1) break;
	}
	if(a > 4294967296 || b > 4294967296)exit(-1);
		 //4294967291
	printf("A: %u B: %u\n", a,b);
	++anzKacheln; ++EL_anz;
	einfuegen(a,b);
}

void transformation(){
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

int benachbart(int a[], int b[]){
	if(abs(b[0] - a[0]) + abs(b[1] - a[1]) == 1) return 1;
	return 0;
}

void test(){

/*
char * test ="1 1\n"
			 "2 1\n"
			 "2 2\n"
			 "3 2\n";
*/
char * test ="0 0\n"
			 "0 1\n"
			 "0 3\n"
			 "1 0\n"
			 "1 2\n"
			 "1 3\n";

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

	//printf("%s\n",eingabe);

	strcat(eingabe, test);

	//printlist();
}

void funktionstest1(){
	int a[] = {1,1};
	int b[] = {0,2};

	printf("benachbart(a,b): %d\n",benachbart(a,b));
}

void funktionstestH(){



}

//n*|raeuem| loesung
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


void raeume_linearH(){
	int index = 0;
		int gruppiert = 0;

		while(gruppiert < EL_anz){
			//schreibe erstes ELement (nicht (-1,-1) in neue Gruppe
				for(int k=0; k<EL_anz; k++){
					if(!(Element_liste[k][0] == -1 && Element_liste[k][1] == -1)){
						if(H_abfrage_gruppiert(Element_liste[k][0],Element_liste[k][1]) > -1)continue;
						Raeume[index][0][0] = Element_liste[k][0];
						Raeume[index][0][1] = Element_liste[k][1];
					    ++R_anz[index];

						H_gruppieren(Element_liste[k][0],Element_liste[k][1]);
					    Element_liste[k][0] =-1; Element_liste[k][1] =-1;
					    ++gruppiert;
					    break;
					}
				}

				//printf("gruppe(%d): (%d,%d)\n", index,Raeume[index][0][0], Raeume[index][0][1]);

				int i=0;
				int k=0;
				while(i <= R_anz[index]){
					//printf("		Betrachte: (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1]);
					//linker Nachbar?
						k = H_suchen(Raeume[index][i][0],Raeume[index][i][1]-1);
						if(k!=-1){
							//printf("		Nachbarn: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);
							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("		Nachbarn eintragen: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}
					//rechter Nachbar?
						k = H_suchen(Raeume[index][i][0],Raeume[index][i][1]+1);
						if(k!=-1){
							//printf("		Nachbarn: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("		Nachbarn eintragen: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}
					//oberer Nachbar?
						k = H_suchen(Raeume[index][i][0]+1,Raeume[index][i][1]);
						if(k!=-1){
							//printf("		Nachbarn: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("		Nachbarn eintragen: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}
					//unterer Nachbar?
						k = H_suchen(Raeume[index][i][0]-1,Raeume[index][i][1]);
						if(k!=-1){
							//printf("		Nachbarn: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

							//schon in Raum eingetragen?
							if(H_abfrage_gruppiert(Hash_liste[k][0],Hash_liste[k][1]) > -1){

							}
							else{
								//printf("		Nachbarn eintragen: (%d,%d) (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1],Hash_liste[k][0],Hash_liste[k][1]);

								Raeume[index][R_anz[index]][0] = Hash_liste[k][0];
								Raeume[index][R_anz[index]][1] = Hash_liste[k][1];
								H_gruppieren(Hash_liste[k][0],Hash_liste[k][1]);
								++R_anz[index]; ++gruppiert;
							}
							if(gruppiert == anzKacheln)break;
						}

						//printf("		i=%d / Anzahl Element(Raum)=%d\n",i,R_anz[index]);

						++i;
				}

			//sortiere (notwendig?)
			//sort();
			++index;
		}

}


int main(void) {
	setbuf(stdout, NULL); //Printout bug lösen
	init();
	H_init();

	//funktionstestH();exit(0);





	einlesen();

	//test();


	transformation();
	H_hashtabelle_erstellen();


	//sort();



	//printlist();
	//H_printHash();

	raeume_linearH();
	//raeume_prim();

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
