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

#include <math.h>


//TODO: (0,0) kann nicht gehashed werden! => work around

int Kollisionen= 0;

//Hashing
	int maxNEXT = 10;
	int H_belegt = 0;
	int H_gruppiert = 0;
	int H_geloest= 0;
	int gruppiert = 0;

	int H_P = 9999830;

	unsigned int Hash_liste[9999830][2]; //TODO: variabel machen (anzKacheln)
	unsigned int Hash_gruppiert[9999830][2];
	unsigned int Hash_geloest[9999830][2];
	unsigned int maxNEXT_liste = 1;
	unsigned int maxNEXT_gruppiert = 1;
	unsigned int maxNEXT_geloest = 1;


	int H_seed = 777;

	unsigned int Element_listeR[999983][2];
	unsigned int Loesung_R[999983][4]; 	int index_loesung=0;


//Adjazensliste + sort
	unsigned int Element_liste[999983][2]; int EL_anz;
	unsigned int Loesung_liste[999983][4]; unsigned int loesungsindex = 0;
	unsigned int Raeume[1000][100000][2]; int R__El_anz[100];
	unsigned int Loesung_Raeume[100][100000][4]; int R__El_anz[100];


	unsigned int anzKacheln;
	int position=0;
	size_t liste_size = 10000;

	void transformation();
	int loesungsschritt(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung, int x, int y, int i);


int quersumme(int k){
	int q=0;
	while(k > 0){
		q+=k%10;
		k/=10;
	}
	return q;
}

unsigned int H_HP_berechnen(unsigned int x,unsigned int y){
	//printf("HS berechnen: (%u,%u)\n", x,y);
	int k = 0;
	k =  (1000*x + 10* y) % H_P;
	//k += ((x << 19) | (y << 7)) % H_P;
	k += ((int) sqrt(x*x+y*y)) % H_P;
	//k = (((x+y)*(x+y+1)/2)+y)% H_P;
	//k += ((x+1 * x)+(y+1 * y)) % H_P;
	//k^=323522*(x*y);
	return k;
}

unsigned int H_next_berechnen(unsigned int k, unsigned int next){
	k =  (k*1000+next*10) % H_P;
//	k^=323522*(x*y);
	return k;
}


//HS: (x,y) -> 1000*x + 10*y % P
void H_init(){
	for(int i=0; i<H_P; i++){
		Hash_liste[i][0] = 0; Hash_liste[i][1] = 0;
		Hash_gruppiert[i][0] = 0; Hash_gruppiert[i][1] = 0;
		Hash_geloest[i][0] = 0; Hash_geloest[i][1] = 0;
	}
}


void H_gruppieren(unsigned int x,unsigned int y){
	//printf("		gruppiere: (%d,%d)\n:",x,y);
	if(H_gruppiert == H_P){
		printf("Hashtabelle gruppieren voll!");
		exit(-1);
	}
	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_gruppiert[k][0] == 0 && Hash_gruppiert[k][1] == 0){
		Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
		++H_gruppiert;
		return;
	}
	else {
		unsigned int next=1;
		while(1){
			printf("%d Kollision (gruppiert) (%u,%u) an k=%d\n",next,x,y,k);
			k = H_next_berechnen(k,next);
			if(Hash_gruppiert[k][0] == 0 && Hash_gruppiert[k][1] == 0){
				Hash_gruppiert[k][0] = x; Hash_gruppiert[k][1] = y;
				++H_gruppiert;
				if(next > maxNEXT_gruppiert)maxNEXT_gruppiert = next;
				return;
			}
			if(next == H_P){
				printf("B Hashtabelle zu dicht belegt! (%d)",H_gruppiert);
				exit(-1);
			}
			++next;
		}
	}
}

void H_loesen(unsigned int x, unsigned int y){
//	printf("		H_loesen eintragen: (%u,%u)\n:",x,y);
	if(H_geloest == H_P){
		printf("Hashtabelle loesen voll!");
		exit(-1);
	}
	unsigned int k =H_HP_berechnen(x,y);
	if(Hash_geloest[k][0] == 0 && Hash_geloest[k][1] == 0){
		Hash_geloest[k][0] = x; Hash_geloest[k][1] = y;
		++H_geloest;
		return;
	}
	else {
		unsigned int next=1;
		while(1){
			printf("%d Kollision (geloest) (%u,%u) an k=%d\n",next,x,y,k);
			k = H_next_berechnen(k,next);
			if(Hash_geloest[k][0] == 0 && Hash_geloest[k][1] == 0){
				Hash_geloest[k][0] = x; Hash_geloest[k][1] = y;
				++H_geloest;
				if(next > maxNEXT_geloest)maxNEXT_geloest = next;
			}
			if(next == H_P){ //problem: max. def. dichte erreicht => abbruch
				printf("C Hashtabelle zu dicht belegt! ()");
				exit(-1);
			}
			++next;
		}
	}
}

void H_loesen_loeschen(unsigned int x, unsigned int y){
	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_geloest[k][0] == x && Hash_geloest[k][1] == y){
		Hash_geloest[k][0] = 0; Hash_geloest[k][1] = 0;
		--H_geloest;
	}
	else {
		//inplace hashing
		unsigned int next=1;
		while(!(Hash_geloest[H_next_berechnen(k,next)][0] != x && Hash_geloest[H_next_berechnen(k,next)][1] != y)){
				if(next == maxNEXT_geloest){ //problem: max. def. dichte erreicht => abbruch
				printf("H_loesen_loeschen Fehler => ELement (%d,%d) verschwunden",x,y);
				exit(-1);
			}
			++next;
		}
		Hash_geloest[k][0] = 0; Hash_geloest[k][1] = 0;
		--H_geloest;
	}
}

void H_eintragen(unsigned int x, unsigned int y){
	if(H_belegt == H_P){
		printf("Hashtabelle Liste voll!");
		exit(-1);
	}
	unsigned int k = H_HP_berechnen(x,y);
	//printf("trage in HL ein: (%u,%u)\n",x,y);
	if(Hash_liste[k][0] == 0 && Hash_liste[k][1] == 0){
		Hash_liste[k][0] = x; Hash_liste[k][1] = y;
		++H_belegt;
	}
	else {
		unsigned int next=1;
		while(1){
			printf("%d Kollision (eintragen) (%u,%u) an k=%d\n",next,x,y,k);
			k = H_next_berechnen(k,next);
			if((Hash_liste[k][0] == 0 && Hash_liste[k][1] == 0)){
				Hash_liste[k][0] = x; Hash_liste[k][1] = y;
				++H_belegt;
				return;
			}
			if(next == H_P){ //problem: max. def. dichte erreicht => abbruch
				printf("A Hashtabelle zu dicht belegt!");
				exit(-1);
			}
			++next;
		}
	}
}

void H_hashtabelle_erstellen(){
	for(int index=0; index<anzKacheln; ++index){
		 //printf("HS eintragen: (%u,%u)\n",Element_liste[index][0], Element_liste[index][1] );
			H_eintragen(Element_liste[index][0], Element_liste[index][1]);
	}
}

int H_suchen(unsigned int x, unsigned int y){
	//printf("				suche: (%d,%d)\n",x,y);
	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_liste[k][0] == x && Hash_liste[k][1] ==y){
		return 1;
	}
	else {
		//inplace hashing
		unsigned int next=1;
		while(1){
			k = H_next_berechnen(k,x);
			if(Hash_liste[k][0] == x && Hash_liste[k][1] == y) return 1;
			if(next == maxNEXT_liste){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
	}
}

int H_abfrage_gruppiert(unsigned int x, unsigned int y){
	//printf("		grupiert?: (%d,%d)\n:",x,y);

	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_gruppiert[k][0] == x && Hash_gruppiert[k][1] ==y){
		return 1;
	}
	else {
		//inplace hashing
		unsigned int next=1;
		while(1){
			k = H_next_berechnen(k,x);
			if(Hash_gruppiert[k][0] == x && Hash_gruppiert[k][1] == y) return 1;
			if(next == maxNEXT_gruppiert){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
	}
}

int H_abfrage_geloest(unsigned int x, unsigned int y){
	//printf("		grupiert?: (%d,%d)\n:",x,y);

	unsigned int k = H_HP_berechnen(x,y);
	if(Hash_geloest[k][0] == x && Hash_geloest[k][1] ==y){
		return 1;
	}
	else {
		//inplace hashing
		unsigned int next=1;
		while(1){
			k = H_next_berechnen(k,x);
			if(Hash_geloest[k][0] == x && Hash_geloest[k][1] == y) return 1;
			if(next == maxNEXT_geloest){ //problem: max. def. dichte erreicht => abbruch
				return -1;
			}
			++next;
		}
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
	if(a == 0 && b == 0){
		fprintf(stderr, "(0,0) eingelesen, NULL-Problem loesen!");
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

int raeume_linearH_schritt(int index, int i, int gruppiert, int x,int y){
 	int	k = H_suchen(Raeume[index][i][0]+x,Raeume[index][i][1]+y);
	if(k!=-1){
		//schon in Raum eingetragen?
		if(H_abfrage_gruppiert(Raeume[index][i][0]+x,Raeume[index][i][1]+y) > -1){

		}
		else{
			//printf("		Nachbarn eintragen: (%u,%u) (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1],Raeume[index][i][0]+x,Raeume[index][i][1]+y);

			//printf("%d Nachbarn: (%u,%u) (%u,%u)\n",gruppiert,Raeume[index][i][0],Raeume[index][i][1],Raeume[index][i][0]+x,Raeume[index][i][1]+y);
			Raeume[index][R__El_anz[index]][0] = Raeume[index][i][0]+x;
			Raeume[index][R__El_anz[index]][1] = Raeume[index][i][1]+y;
			H_gruppieren(Raeume[index][i][0]+x,Raeume[index][i][1]+y);
			return 1;
		}
	}
	return 0;

}

void raeume_linearH(){
	if(anzKacheln % 2 == 1){
		printf("Keine Loesung moeglich (ungerade Anzahl Kacheln!)\n");
		exit(0);
	}

	printf("berechne raeume ...\n");

	int index = 0;

		while(gruppiert < EL_anz){
			//schreibe erstes ELement (nicht (-1,-1) in neue Gruppe
				for(int k=0; k<EL_anz; k++){
					// if(!(Element_liste[k][0] == 0 && Element_liste[k][1] == 0)){
						if(H_abfrage_gruppiert(Element_liste[k][0],Element_liste[k][1]) > -1)continue;

						Raeume[index][0][0] = Element_liste[k][0];
						Raeume[index][0][1] = Element_liste[k][1];
					  ++R__El_anz[index];

						H_gruppieren(Element_liste[k][0],Element_liste[k][1]);
					  printf("gruppiert / EL_anz: %u %u\n",gruppiert,EL_anz);
						//printf("gruppe(%d): repraesentant: (%u,%u), gruppiert: %d\n", index,Raeume[index][0][0], Raeume[index][0][1],gruppiert);


				    // Element_liste[k][0] =-1; Element_liste[k][1] =-1;
				    ++gruppiert;
				    break;
				}
				// }


				int i=0;
				while(i <= R__El_anz[index]){
					//if(gruppiert % 100 <5)printf("raeume: %d / %d",gruppiert,anzKacheln);

					//printf("		Betrachte: (%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1]);

					//linker Nachbar?
					if(raeume_linearH_schritt(index,  i,  gruppiert, 0, 1) == 1){
						++gruppiert;
						++R__El_anz[index];
					}
						if(gruppiert == anzKacheln)break;
					//rechter Nachbar?
					if(raeume_linearH_schritt(index,  i,  gruppiert, 0, -1) == 1){
						++gruppiert;
						++R__El_anz[index];
					}
						if(gruppiert == anzKacheln)break;
					//oberer Nachbar?
					if(raeume_linearH_schritt(index,  i,  gruppiert, 1, 0) == 1){
						++gruppiert;
						++R__El_anz[index];
					}
						if(gruppiert == anzKacheln)break;
					//unterer Nachbar?
					if(raeume_linearH_schritt(index,  i,  gruppiert, -1, 0) == 1){
							++gruppiert;
							++R__El_anz[index];
					}
						if(gruppiert == anzKacheln)break;

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
	int sum_l=0;
	for(int i=0; i<loesungsindex; i++){
		if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)break;
		sum_l+=Loesung_liste[i][0]+Loesung_liste[i][1]+Loesung_liste[i][2] +Loesung_liste[i][3];
		if(abs(Loesung_liste[i][0] - Loesung_liste[i][2])  +	abs(Loesung_liste[i][1] -	Loesung_liste[i][3]) != 1){
			printf("FALSCH: %d %d;%d %d\n", Loesung_liste[i][0],Loesung_liste[i][1],Loesung_liste[i][2],Loesung_liste[i][3]);
			return 0;
		}
	}
	int sum_e=0;
	for(int k=0; k<anzKacheln; k++){
		sum_e += Element_liste[k][0]+Element_liste[k][1];
	}
	if(sum_e != sum_l)return 0;
	return 1;
}

int validate2(){
	int drin = 0;
	int loesungsindex_ = 0;
	for(int i=0; i<999983; i++){
		if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)continue;
		//printf("%u %u;%u %u\n", Loesung_liste[i][0],Loesung_liste[i][1],Loesung_liste[i][2],Loesung_liste[i][3]);
			++loesungsindex_;
			drin = 0;
			for(int k=0; k<anzKacheln; k++){
				if(Loesung_liste[i][0] == Element_liste[k][0] && Loesung_liste[i][1] == Element_liste[k][1]){
					drin = 1; break;
				}
				if(Loesung_liste[i][2] == Element_liste[k][0] &&	Loesung_liste[i][3] == Element_liste[k][1]){
					drin = 1; break;
				}
			}
			if(drin == 0)printf("(%u,%u) nicht in ElementDat\n",Loesung_R[i][0], Loesung_R[i][1]);
	}

	for(int k=0; k<anzKacheln;k++){
		//printf("%d: elementcheck_loesung: (%d,%d)\n", k,Element_liste[k][0], Element_liste[k][1]);
		drin = 0;
		for(int i =0; i<999983; i++){
			if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)continue;
			//printf("%u %u;%u %u\n", Loesung_liste[i][0],Loesung_liste[i][1],Loesung_liste[i][2],Loesung_liste[i][3]);
				//++loesungsindex_;
					if(Loesung_liste[i][0] == Element_liste[k][0] && Loesung_liste[i][1] == Element_liste[k][1]){
						drin = 1; break;
					}
					if(Loesung_liste[i][2] == Element_liste[k][0] && Loesung_liste[i][3] == Element_liste[k][1]){
						drin = 1; break;
					}
		}
		if(drin == 0)printf("(%u,%u) nicht in Loesung!\n",Element_liste[k][0],Element_liste[k][1]);
	}

	int dopplungen =0;
	for(int i=0; i<999983; i++){
		if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)continue;
		for(int j=i+1; j<999983; j++){
			if(Loesung_liste[j][0] == 0 &&	Loesung_liste[j][1] == 0 && Loesung_liste[j][2] == 0 &&	Loesung_liste[j][3] == 0)continue;

			if(Loesung_liste[i][0] == Loesung_liste[j][0] &&	Loesung_liste[i][1] == Loesung_liste[j][1]){
			  printf("1 (%u,%u) doppelt in Loesung!\n",Loesung_liste[j][0],Loesung_liste[j][1]);
				++dopplungen;
				Loesung_liste[i][0] = Loesung_liste[j][0] = Loesung_liste[i][1] = Loesung_liste[j][1] = 0;
				continue;
			}
			if(Loesung_liste[i][0] == Loesung_liste[j][2] &&	Loesung_liste[i][1] == Loesung_liste[j][3]){
				printf("2 (%u,%u) doppelt in Loesung!\n",Loesung_liste[j][2],Loesung_liste[j][3]);
				++dopplungen;
				Loesung_liste[i][0] = Loesung_liste[j][2] = Loesung_liste[i][1] = Loesung_liste[j][3] = 0;
				continue;
			}
			if(Loesung_liste[i][2] == Loesung_liste[j][0] &&	Loesung_liste[i][3] == Loesung_liste[j][1]){
				printf("3 (%u,%u) doppelt in Loesung!\n",Loesung_liste[j][0],Loesung_liste[j][1]);
				++dopplungen;
				Loesung_liste[i][2] = Loesung_liste[j][0] =	Loesung_liste[i][3] = Loesung_liste[j][1] = 0;
				continue;
			}
			if(Loesung_liste[i][2] == Loesung_liste[j][2] &&	Loesung_liste[i][3] == Loesung_liste[j][3]){
				printf("4 (%u,%u) doppelt in Loesung!\n",Loesung_liste[j][0],Loesung_liste[j][1]);
				++dopplungen;
				Loesung_liste[i][2] = Loesung_liste[j][2] = Loesung_liste[i][3] = Loesung_liste[j][3] = 0;
				continue;
			}
			//i0 i1 i2 i3
			//j0 j1 j2 j3
		}

	}


	return 1;
}

int validate_raeume(){
	//ist jedes Element aus Raemen in Element
	int anzraumkacheln = 0;
	int raum = 0;
	while(R__El_anz[raum] != 0){
		for(int i=0; i<R__El_anz[raum]; i++){
			if(Raeume[raum][i][0] != -1 && Raeume[raum][i][0] != -1){
				int k;
				for(k=0; k<anzKacheln; k++){
					if(Raeume[raum][i][0] == Element_liste[k][0] &&	Raeume[raum][i][1] == Element_liste[k][1]){
						++anzraumkacheln;
						break;
					}
				}
				if(k == anzKacheln)printf("(%d,%d) (%d)aus Raum %d nicht in Element\n",Raeume[raum][i][0], Raeume[raum][i][1],k,raum);
			}
		}
		++raum;
	}
	printf("%d / %d in raeumen verteilt!\n",anzraumkacheln,anzKacheln);

	int k;
	for(k=0; k<anzKacheln; k++){
		if(Element_liste[k][0] != -1 && Element_liste[k][0] != -1){
			while(R__El_anz[raum] != 0){
					int i;
					for(i=0; i<R__El_anz[raum]; i++){
							if(Raeume[raum][i][0] == Element_liste[k][0] &&	Raeume[raum][i][0] == Element_liste[k][1]){
								++anzraumkacheln;
								break;
							}
					}
					if(i == anzKacheln)printf("(%d,%d) (%d)aus Element %d nicht in Raum\n",Element_liste[k][0], Element_liste[k][1],k,raum);
			}
			++raum;
		}
	}
	printf("%d / %d in raeumen verteilt!\n",anzraumkacheln,anzKacheln);


	return 1;
}


int loesung_prim(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung){
	printf("raum: %d, %d/ %d  Elemente geloest \n",raum,index_loesung,R__El_anz[raum]/2);
	//gueltige loesung: basisfall
	if(index_loesung== R__El_anz[raum]/2){
		printf("loesung errechnet, %d\n", index_loesung);
		for(int i=0; i<index_loesung;i++){
			printf("%u %u;%u %u\n", Loesung_R[i][0],Loesung_R[i][1],Loesung_R[i][2],Loesung_R[i][3]);
			Loesung_liste[loesungsindex][0] = Loesung_R[i][0];
			Loesung_liste[loesungsindex][1] = Loesung_R[i][1];
			Loesung_liste[loesungsindex][2] = Loesung_R[i][2];
			Loesung_liste[loesungsindex][3] = Loesung_R[i][3];
			++loesungsindex;
			//loesung[]
		}
		return 1;
	}

		int suche=0;


		int i;
		for(i=0; i<R__El_anz[raum]; i++){
						//if(Raeume[raum][i][0] ==-1 && Raeume[raum][i][1] == -1)continue;
		//1. nehme erste verbleibende in Element_liste
					//pruefe ob kachel aus Element_liste schon in loesung
					if(index_loesung == 0){
						printf("\nraum %d, starte bei: %d\n",raum,i);
						//Hash_geloest
						// for(int i=0; i<999983; i++){
						// 	Hash_geloest[i][0] = 0; Hash_geloest[i][1] = 0;
						// 	//if(Hash_geloest[i][0] != 0 || Hash_geloest[i][1] != 0)++hashplaetze;
						// }
						printf("hashbelegungen %d\n\n",maxNEXT_geloest);
					}

					int geloest;
					geloest = H_abfrage_geloest(Raeume[raum][i][0], Raeume[raum][i][1]);
					if(geloest != -1){
						continue;
					}
					printf("waehle: (%u,%u) (gruppiert:%d)\n", Raeume[raum][i][0], Raeume[raum][i][1],index_loesung);

					// Raeume[raum][i][0] = -1;
					// Raeume[raum][i][1] = -1;



	 //2. suche Nachbarn
					//oberer Nachbarn?
					suche = H_suchen(Raeume[raum][i][0], Raeume[raum][i][1]+1);
					if(suche != -1){
							int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, 0 , 1,i);
							if(ret == 1) return 1;
					}

					//unterer Nachbarn?
					suche = H_suchen(Raeume[raum][i][0], Raeume[raum][i][1]-1);
					if(suche != -1){
							int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, 0 , -1,i);
							if(ret == 1) return 1;
					}

					//rechter Nachbarn?
					suche = H_suchen(Raeume[raum][i][0]+1, Raeume[raum][i][1]);
					if(suche != -1){
						int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, 1, 0,i);
						if(ret == 1) return 1;
					}

					//linker Nachbarn?
					suche = H_suchen(Raeume[raum][i][0]-1, Raeume[raum][i][1]);
					if(suche != -1){
						int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, -1 , 0,i);
						if(ret == 1) return 1;
					}
					printf("keinen nachbar fuer (%u,%u) (gruppiert: %d)\n", Raeume[raum][i][0], Raeume[raum][i][1],index_loesung);
					if(index_loesung > 0)return 0;
		}//Ende FOR Schleife
		// if(sprung == anzKacheln-1)return 2;
		return 2;
}

int loesungsschritt(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung, int x, int y, int i){
				//TODO: Funktion
				//pruefe ob kachel aus Element_liste schon in loesung
				int geloest = H_abfrage_geloest(Raeume[raum][i][0]+x, Raeume[raum][i][1]+y);
				if(geloest == -1){
				 printf("%d%d		neue Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",x,y, Raeume[raum][i][0], Raeume[raum][i][1], Raeume[raum][i][0]+x, Raeume[raum][i][1]+y,index_loesung);

					H_loesen(Raeume[raum][i][0], Raeume[raum][i][1]);
					H_loesen(Raeume[raum][i][0]+x, Raeume[raum][i][1]+y); //TODO: mgl. Raeume[?][i][0,1] herrausfinden


					loesung[index_loesung][0] = Raeume[raum][i][0];
					loesung[index_loesung][1] = Raeume[raum][i][1];
					loesung[index_loesung][2] = Raeume[raum][i][0]+x;
					loesung[index_loesung][3] = Raeume[raum][i][1]+y;

					++index_loesung;
					++index_loesung_raum;

					int l = loesung_prim(raum, loesung, index_loesung_raum,index_loesung);

					if(l == 1) return 1;
					else{
						H_loesen_loeschen(Raeume[raum][i][0], Raeume[raum][i][1]);
						H_loesen_loeschen(Raeume[raum][i][0]+x, Raeume[raum][i][1]+y);
						//teilloesung nicht valide => rollback
						loesung[index_loesung][0] = 0;
						loesung[index_loesung][1] = 0;
						loesung[index_loesung][2] = 0;
						loesung[index_loesung][3] = 0;

						--index_loesung;
						--index_loesung_raum;
					}
				}
				return 2;
}


// void loesung_ober(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung){
// 	int sprung = 0;
// 	while(1){
// 		for(int i=0; i<500000; i++){
// 			if(Hash_geloest[i][0] != 0 || Hash_geloest[i][1] != 0){
// 				//printf("hash nicht leer (%d,%d)!!!\n", Hash_geloest[i][0], Hash_geloest[i][1]);
// 				//exit(0);
// 				Hash_geloest[i][0] = 0; Hash_geloest[i][1] = 0;
// 			}
// 		}
// 		printf("\n\n\nEbene 0, Sprung: %d", sprung);
// 		if(sprung == R__El_anz[raum]-1)break;
// 		if(loesung_prim(raum, loesung, 0,index_loesung,sprung) == 1)break;
// 		++sprung;
// 	}
// }


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
	printf("hs angefertigt ... %d Eintraege, %d Kollisionen!\n",H_belegt,Kollisionen);

	//sort();

	//H_printHash();

	raeume_linearH();
//	raeume_prim();

	validate_raeume();
	printraeume();

//	exit(0);


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



	for(int raum = 0; ;++raum){
		if(R__El_anz[raum] == 0)break;
		for(int i=0; i<anzKacheln; i++){
			Loesung_R[i][0] = 0;
			Loesung_R[i][1] = 0;
			Loesung_R[i][2] = 0;
			Loesung_R[i][3] = 0;
		}
		printf("loese raum %d\n", raum);
		loesung_prim(raum, Loesung_R, 0,index_loesung);
		//loesung_ober(raum, Loesung_R, 0,index_loesung);
	}

  printf("\n\nLoesung:\n");
	for(int i=0; i<loesungsindex; i++){
		if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)continue;
		printf("%u %u;%u %u\n", Loesung_liste[i][0],Loesung_liste[i][1],Loesung_liste[i][2],Loesung_liste[i][3]);
	}

	printf("index_loesung: %u\n",index_loesung);

	printf("validate1 =%d\n",validate());
	printf("validate2 =%d\n",validate2());

	printf("anzKacheln: %d\n",anzKacheln );

	exit(0);
	//funktionstest1();
	return EXIT_SUCCESS;
}
