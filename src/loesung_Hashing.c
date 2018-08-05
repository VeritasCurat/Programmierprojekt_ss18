/*
 ============================================================================
 Name        : loesung_hashing.c
 Author      : Johannes Grobelski
 Version     :
 Copyright   : Your copyright noticeo,,,
 Description : Programmierprojekt SS18
 ============================================================================
 */

#include "test.c"//TODO: entfernen


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



  int debug = 0;

//TODO: (0,0) kann nicht gehashed werden! => work around


//Hashing
  int gruppiert = 0;

  unsigned int Element_listeR[999983][2];
  unsigned int Loesung_R[999983][4]; 	int index_loesung=0;


//Adjazensliste + sort
  unsigned int Element_liste[999983][2]; int EL_anz;

  int gruppiert_liste[999983];
  int geloest_liste[999983];


  unsigned int Loesung_liste[999983][4]; unsigned int loesungsindex = 0;
  unsigned int Raeume[1000][100000][2]; int R__El_anz[100];
  unsigned int Loesung_Raeume[100][100000][4]; int R__El_anz[100];


  unsigned int anzKacheln;
  int position=0;
  size_t liste_size = 999983;

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

void init(){
//	int (*Element_liste)[2] = malloc(sizeof(int[liste_size][2])); //TODOfree(Element_liste)

  for(int i=0; i<anzKacheln; i++){
    Element_liste[i][0] = 0; Element_liste[i][1] = 0;
    geloest_liste[i]=0;
    gruppiert_liste[i]=0;
  }

  for(int raum=0; raum<100; raum++){
    for(int i=0; i<liste_size; i++){
      Raeume[raum][i][0] = 0; Raeume[raum][i][1] = 0;
    }
  }
}



void printraeume(){
  int index =0;
  while(R__El_anz[index] != 0){
  printf("Raum %d (%d):\n",index,R__El_anz[index]);
  for(int i=0; i<R__El_anz[index]; i++){
  printf("x: %d, y: %d\n", Raeume[index][i][0], Raeume[index][i][1]);
  }

  ++index;
  }
}

int suche(unsigned int x, unsigned int y){
  for(int i=0; i< anzKacheln; i++){
    if(Element_liste[i][0] == x && Element_liste[i][1] == y)return i;
  }
  return -1;
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

void inttostring(char b[11], unsigned int x){
  int cnt=0;
  //falsch herum einlesen
  if(x == 0){strcpy(b,"0000000000");}
  while(cnt < 10){
    b[cnt] = x%10 + '0';
    x/=10;
    ++cnt;
  }

  //umdrehen
  for(int i = 0; i < 6; ++i){
    char c = b[i];
    b[i] = b[cnt-i-1];
    b[cnt-i-1] = c;
  }
  //b[cnt] = '\0';


  //sprintf(b, "%u", x);
}


/**
Bijektive Abbildung: (x,y) -> xy
*/
void binMeld(char res[22], unsigned int x, unsigned int y){
//printf("      meld: (%u, %u)\n",x,y);



  char a[11]="";
  char b[11]="";

  inttostring(a,x);
  inttostring(b,y);

  //printf("x = %s\n",a);
  //printf("y = %s\n",b);

  int cnt =0;
  for(int i=0; i<10; i++){
    if(a[i] != '\0'){res[cnt] = a[i];++cnt;}
  }
  for(int i=0; i<10; i++){
    if(b[i] != '\0'){res[cnt] = b[i];++cnt;}
  }

  // for(cnt;cnt<22; cnt++){
  //   res[cnt] = '0';
  // }
  res[21]='\0';

  // for(int i=0; i<22; i++){
  //   printf("%d: %c\n",i,res[i]);
  // }

}

int binCompare(unsigned int x1, unsigned int y1, unsigned  int x2, unsigned int y2){
  char q[22] = "";
  char p[22] = "";

  binMeld(q, x1,y1);
  binMeld(p, x2,y2);

  if(debug == 1)  printf("q = %s\np = %s\n",q,p );

  int l=0; int m=0;
  for(int i=0; i<20; i++){
    //printf("%d: %c %c\n",i,a[i],b[i]);
     int a_ = chartoint(q[i]);
     int b_ = chartoint(p[i]);
     //printf("%d: a_=%d b_=%d\n",i,a_,b_);
     if(a_ > b_)return 1;
     if(a_ < b_)return -1;
  }

  // if(_1 < _2)return -1;
  // else if(_1 > _2) return 1;
   return 0;
}

int binSearch(unsigned int x, unsigned int y){
  int l=0; int r=anzKacheln-1; int m;
  while(l <= r){
    m = (int) ((r+l)/2);
    if(binCompare(Element_liste[m][0], Element_liste[m][1], x, y) < 0){
      if(debug == 1){printf("<    l=%d, r=%d, m=%d\n",l,r,m );}
      l = m+1;
      continue;
    }
    else if(binCompare(Element_liste[m][0], Element_liste[m][1], x, y) > 0){
      if(debug == 1)printf(">    l=%d, r=%d, m=%d\n",l,r,m );
      r = m-1;
      continue;
    }
    else if(binCompare(Element_liste[m][0], Element_liste[m][1], x, y) == 0)break;
  }
  //if(binCompare(Element_liste[m][0], Element_liste[m][1], x, y) == 0)return m;
  if(Element_liste[m][0] == x && Element_liste[m][1] == y)return m;

  else{
    //printf("  l=%d, r=%d, m=%d\n",l,r,m );
    return -1;
  }


}

void printlist(){
  for(int i=0; i<anzKacheln; i++){
    char a[22]="";binMeld(a,Element_liste[i][0] , Element_liste[i][1]);
    printf("%u: A: %u, B: %u => %s\n", i, Element_liste[i][0] , Element_liste[i][1],a );
  }
}

void delete(int x, int y){
  int suche_ = binSearch(x,y);
  if(suche > 0){
  Element_liste[suche_][0]=0;
  Element_liste[suche_][1]=0;
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

static int comp1(const void* a, const void* b) {
  int* array1 = (int*) a;
  int* array2 = (int*) b;
  // if(array1[0] > array2[0])return 1;
  // else if(array1[0] < array2[0]) return -1;
  // else return 0;
  int diff1 = array1[0] - array2[0];
  if (diff1) return diff1;
  return array1[1] - array2[1];
}

static int comp(const void* a, const void* b) {
  const unsigned int* ai = ( const unsigned int* )a;
  const unsigned int* bi = ( const unsigned int* )b;

  if( ai[0] < bi[0] )
  {
      return -1;
  }
  else if( ai[0] > bi[0] )
  {
      return 1;
  }
  else
  {  if( ai[1] < bi[1] )
    {
        return -1;
    }
    else if( ai[1] > bi[1] )
    {
        return 1;
    }
    else return 0;
  }
}


void sort(){
   qsort(Element_liste, position, 2*sizeof(int), comp);
}

void sort_raeume(){
  int raum =0;
  while(R__El_anz[raum] > 0){
  qsort(Raeume[raum], R__El_anz[raum], 2*sizeof(int), comp);
  ++raum;
  }
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
int raeume_linearH_schritt(int index, int i, int gruppiert, int x,int y){

  printf("(%u,%u) Nachbar?\n", Raeume[index][i][0]+x,Raeume[index][i][1]+y);
 	int	k = binSearch(Raeume[index][i][0]+x,Raeume[index][i][1]+y);
  if(k!=-1){
    //schon in Raum eingetragen?
    printf("(%u,%u) Nachbar gruppiert?\n", Raeume[index][i][0]+x,Raeume[index][i][1]+y);

    if(gruppiert_liste[k] == 0){
      printf("(%u,%u) Nachbar eintragen\n", Raeume[index][i][0]+x,Raeume[index][i][1]+y);

      Raeume[index][R__El_anz[index]][0] = Raeume[index][i][0]+x;
      Raeume[index][R__El_anz[index]][1] = Raeume[index][i][1]+y;
      gruppiert_liste[k] = 1;
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

  //printf("berechne raeume ...\n");

  int index = 0;

  while(gruppiert < anzKacheln){
    for(int k=0; k<anzKacheln; k++){
    if(gruppiert_liste[k] == 1)continue;

      Raeume[index][0][0] = Element_liste[k][0];
      Raeume[index][0][1] = Element_liste[k][1];
      ++R__El_anz[index];

      gruppiert_liste[k] = 1;
      // printf("gruppiert / EL_anz: %u %u\n",gruppiert,anzKacheln);
      // printf("gruppe(%d): repraesentant: (%u,%u), gruppiert: %d\n", index,Raeume[index][0][0], Raeume[index][0][1],gruppiert);

      ++gruppiert;
      break;
    }
    // }

    if(gruppiert == anzKacheln)break;

    int i=0;
    int nachbarn =0;
    while(i < R__El_anz[index]){
      //if(gruppiert % 100 <5)printf("raeume: %d / %d",gruppiert,anzKacheln);

      printf("\n\n Betrachte: (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1]);

      //linker Nachbar?
      if(raeume_linearH_schritt(index,  i,  gruppiert, 0, 1) == 1){
        ++gruppiert;
        ++R__El_anz[index];
        ++nachbarn;
      }
      if(gruppiert == anzKacheln)break;
      //rechter Nachbar?
      if(raeume_linearH_schritt(index,  i,  gruppiert, 0, -1) == 1){
        ++gruppiert;
        ++R__El_anz[index];
        ++nachbarn;
      }
      if(gruppiert == anzKacheln)break;
      //oberer Nachbar?
      if(raeume_linearH_schritt(index,  i,  gruppiert, 1, 0) == 1){
        ++gruppiert;
        ++R__El_anz[index];
        ++nachbarn;
      }
      if(gruppiert == anzKacheln)break;
      //unterer Nachbar?
      if(raeume_linearH_schritt(index,  i,  gruppiert, -1, 0) == 1){
        ++gruppiert;
        ++R__El_anz[index];
        ++nachbarn;
      }


      if(gruppiert == anzKacheln)break;

    //  printf("		i=%d / Anzahl Element(Raum)=%d\n",i,R__El_anz[index]);

      ++i;
      nachbarn = 0;
      }
      if(gruppiert == anzKacheln)break;

      if(R__El_anz[index] % 2 == 1){
        printf("Ein Raum hat eine ungerade Anzahl von Kacheln (Raum %d: %d Kacheln)\n",index, R__El_anz[index] );
        for(int i=0; i<R__El_anz[index]; ++i){
          printf("			(%d,%d)\n",Raeume[index][i][0],Raeume[index][i][1]);
        }
        exit(-1);
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
  int valid=1;
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
  if(k == anzKacheln){
            printf("(%d,%d) (%d)aus Raum %d nicht in Element\n",Raeume[raum][i][0], Raeume[raum][i][1],k,raum);
            valid = 0;
        }
  }
  }
  ++raum;
  }
  //printf("%d / %d in raeumen verteilt!\n",anzraumkacheln,anzKacheln);
  if(valid == 0)return valid;

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
  if(i == anzKacheln){
            printf("(%d,%d) (%d)aus Element %d nicht in Raum\n",Element_liste[k][0], Element_liste[k][1],k,raum);
            valid = 0;
          }
  }
  ++raum;
  }
  }
  //printf("%d / %d in raeumen verteilt!\n",anzraumkacheln,anzKacheln);
  if(valid == 0)return valid;

  int raumCHECK =0;
  raum = 0;
  while(R__El_anz[raum] > 0){
  for(int i=0; i<R__El_anz[raum]; ++i){
  for(int j=0; j<R__El_anz[raum]; ++j){
        if(benachbart(Raeume[raum][i],Raeume[raum][j]) == 1){
          raumCHECK+=(i+1);
          // printf("%d benachbart: (%u,%u)(%u,%u)\n",i,Raeume[raum][i][0],Raeume[raum][i][1],Raeume[raum][j][0],Raeume[raum][j][1] );
          break;
        }
  }
  }
    if(raumCHECK != ((R__El_anz[raum] * (R__El_anz[raum]+1))/2)){
      printf("Raum %i nicht zusammenhaengend %u != %u !\n",raum,raumCHECK,((R__El_anz[raum] * (R__El_anz[raum]+1))/2));
      valid = 0;
    }
    ++raum;
    raumCHECK = 0;
  }

  return valid;
}


int loesung_prim(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung){
  //printf("raum: %d, %d/ %d  Elemente geloest \n",raum,index_loesung,R__El_anz[raum]/2);
  //gueltige loesung: basisfall
  if(index_loesung== R__El_anz[raum]/2){
    //printf("loesung errechnet, %d\n", index_loesung);
    for(int i=0; i<index_loesung;i++){
      //printf("%u %u;%u %u\n", Loesung_R[i][0],Loesung_R[i][1],Loesung_R[i][2],Loesung_R[i][3]);
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
    //printf("\nraum %d, starte bei: %d\n",raum,i);
    //Hash_geloest
    // for(int i=0; i<999983; i++){
    // 	Hash_geloest[i][0] = 0; Hash_geloest[i][1] = 0;
    // 	//if(Hash_geloest[i][0] != 0 || Hash_geloest[i][1] != 0)++hashplaetze;
    // }
    //	printf("hashbelegungen %d\n\n",maxNEXT_geloest);
    }

  int geloest;
  geloest = geloest_liste[i];
  if(geloest == 1){
    continue;
  }
  //printf("waehle: (%u,%u) (gruppiert:%d)\n", Raeume[raum][i][0], Raeume[raum][i][1],index_loesung);

  // Raeume[raum][i][0] = -1;
  // Raeume[raum][i][1] = -1;

  //16226

     //2. suche Nachbarn
    //oberer Nachbarn?
    suche = binSearch(Raeume[raum][i][0], Raeume[raum][i][1]+1);
    if(suche != -1){
      int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, 0 , 1,i);
      if(ret == 1) return 1;
    }


    //unterer Nachbarn?
    suche = binSearch(Raeume[raum][i][0], Raeume[raum][i][1]-1);
    if(suche != -1){
      int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, 0 , -1,i);
      if(ret == 1) return 1;
    }


    //rechter Nachbarn?
    suche = binSearch(Raeume[raum][i][0]+1, Raeume[raum][i][1]);
    if(suche != -1){
      int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, 1, 0,i);
      if(ret == 1) return 1;
    }

    //linker Nachbarn?
    suche = binSearch(Raeume[raum][i][0]-1, Raeume[raum][i][1]);
    if(suche != -1){
      int ret = loesungsschritt(raum, loesung,index_loesung_raum, index_loesung, -1 , 0,i);
      if(ret == 1) return 1;
    }

    //printf("keinen nachbar fuer (%u,%u) (gruppiert: %d)\n",Raeume[raum][i][0], Raeume[raum][i][1],index_loesung);
    if(index_loesung==16226) printf("TESTUS5\n");

    if(index_loesung > 0)return 0;
  }//Ende FOR Schleife


  // if(sprung == anzKacheln-1)return 2;
  printf("springe zurueck ...\n");
  return 2;
}

int loesungsschritt(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung, int x, int y, int i){
  //TODO: Funktion
  //pruefe ob kachel aus Element_liste schon in loesung
  int geloest = geloest_liste[i];
  if(geloest == 0){
    //printf("%d%d		neue Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",x,y, Raeume[raum][i][0], Raeume[raum][i][1], Raeume[raum][i][0]+x, Raeume[raum][i][1]+y,index_loesung);


    geloest_liste[i] = 1;
    int nachbar_index = binSearch(Raeume[raum][i][0]+x, Raeume[raum][i][1]+y);
    geloest_liste[nachbar_index]=1;


    loesung[index_loesung][0] = Raeume[raum][i][0];
    loesung[index_loesung][1] = Raeume[raum][i][1];
    loesung[index_loesung][2] = Raeume[raum][i][0]+x;
    loesung[index_loesung][3] = Raeume[raum][i][1]+y;

    ++index_loesung;
    ++index_loesung_raum;

    int l = loesung_prim(raum, loesung, index_loesung_raum,index_loesung);

    if(l == 1) return 1;
    else{
      //printf("%d%d	 Nachbarn loeschen: (%u,%u) (%u,%u) (gruppiert: %d)\n",x,y, Raeume[raum][i][0], Raeume[raum][i][1], Raeume[raum][i][0]+x, Raeume[raum][i][1]+y,index_loesung);
      geloest_liste[i] = 0;
      geloest_liste[nachbar_index] = 0;
      //teilloesung nicht valide => rollback
      loesung[index_loesung][0] = 0;
      loesung[index_loesung][1] = 0;
      loesung[index_loesung][2] = 0;
      loesung[index_loesung][3] = 0;

      --index_loesung;
      --index_loesung_raum;
      //printf("	 Nachbarn wurden geloescht ...\n");
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


void binSearchTEST(){
  printf("binSearch TEST ...");
  int fehlgeschlagen=0;
  for(int i=0; i<anzKacheln; i++){
    //printf("(%u,%u): \n",Element_liste[i][0],Element_liste[i][1]);

    if(binSearch(Element_liste[i][0],Element_liste[i][1]) == -1){
      fehlgeschlagen = 1;
      debug = 1;
      binSearch(Element_liste[i][0],Element_liste[i][1]);
      debug = 0;
      printf("lineare Suche: %d \n",suche(Element_liste[i][0],Element_liste[i][1]));
      printf("nicht gefunden!\n\n\n");
    }
    else {
      continue;
      //printf("gefunden!\n");
    }
  }
  if(fehlgeschlagen == 0)printf("%s\n", " erfolgreich!" );
}

void loesung_sort(){
  init();
  einlesen();
  raeume_linearH();
  sort_raeume();
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
  }

  printf("\n\nLoesung:\n");
  for(int i=0; i<loesungsindex; i++){
    if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)continue;
    printf("%u %u;%u %u\n", Loesung_liste[i][0],Loesung_liste[i][1],Loesung_liste[i][2],Loesung_liste[i][3]);
  }

  printf("index_loesung: %u\n",index_loesung);

  printf("validate1 =%d\n",validate());
  printf("validate2 =%d\n",validate2());
  exit(1);
}
