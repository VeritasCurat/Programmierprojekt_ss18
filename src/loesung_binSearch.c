/*
 ============================================================================
 Name        : loesung_binSearch.c
 Author      : Johannes Grobelski
 Version     :
 Copyright   : Your copyright noticeo,,,
 Description : Programmierprojekt SS18
 ============================================================================
 */
 #include <string.h> //TODO: entfernen

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <stdint.h>

//Phase1: Initialisieren der DS, Einlesen von String und umwandeln in unsigned int Element_liste[][2]:
  void init();
  void einlesen();
  void koordinate_einlesen(char* zeile);
  int chartoint(char c);
  void einfuegen(unsigned int x,unsigned int y);
  int legalesZeichen(char c);

//Phase2: Sortieren der Liste mit Quicksort:
  int binSearch(unsigned int x, unsigned int y);
  static int compare(unsigned int x1, unsigned y1, unsigned int x2, unsigned int y2);
  static int comp(const void* a, const void* b);
  void sort();

//Phase3: Raumeinteilung (Raum = Menge von Kachelplaetzen, sodass alle Kachelplaetze min. einen Nachbar aus dieser Menge haben):
  void sort_raeume();
  int raeume_linearH_schritt(int index, int i, int gruppiert, int x,int y);
  void raeume_linearH();

//Phase4: Loesung berechnen:
  int loesungsschritt(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung, int x, int y, int i);
  int loesung_prim(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung);
  void loesung_binSearch();


int debug2 = 0;
int debug3 = 1;
int debug4 = 0;


//Container fuer Vektoren
unsigned int* Element_liste[2]; int EL_anz = 0; int EL_platz = 1000;//hier werden alle Koordinaten gespeichert
unsigned int Raeume[1000][100000][2]; int R__El_anz[100]; int gruppiert = 0;

unsigned int Loesung_R[1000][4]; 	int index_loesung=0; //
unsigned int Loesung_liste[999983][4]; unsigned int loesungsindex = 0;
unsigned int Loesung_Raeume[100][100000][4]; int R__El_anz[100];


//markierung: Kachel in Raum, in loesung?
int gruppiert_liste[999983];
int geloest_liste[999983];

unsigned int anzKacheln;
size_t liste_size = 999983;


void init(){
  int* Element_liste = (int *)malloc(EL_anz * 2 * sizeof(int)); //TODO: free(Element_liste

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

  for(int i=0; i<anzKacheln; i++){
    Loesung_R[i][0] = 0;
    Loesung_R[i][1] = 0;
    Loesung_R[i][2] = 0;
    Loesung_R[i][3] = 0;
  }
}

void einlesen(){
  char buffer[1000000];
  while(fgets(buffer, sizeof(buffer), stdin) != NULL){
    koordinate_einlesen(buffer);
  }
}

void koordinate_einlesen(char* zeile){
  unsigned int a,b; a=b=0;
  int a_len = 0; int b_len =0;

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
  // if(a == 0 && b == 0){ //TODO: nicht mehr in binSearch vorhanden?
  //   fprintf(stderr, "(0,0) eingelesen, NULL-Problem loesen!");
  //   exit(-1);
  // }
  //printf("A: %u B: %u\n", a,b);
  ++anzKacheln; ++EL_anz;
  einfuegen(a,b);
}

int legalesZeichen(char c){
  if(chartoint(c) >= 0 && chartoint(c) <= 9)return 1;
  if(c == ' ' || c == '\n') return 1;
  return 0;
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

void einfuegen(unsigned int x,unsigned int y){
  Element_liste[EL_anz][0] = (unsigned int) x;
  Element_liste[EL_anz][1] = (unsigned int) y;
  ++EL_anz;
}
//Phase2

//liest unsigned int, gibt 11 stelligen String repraesentant aus
int binSearch(unsigned int x, unsigned int y){
  int l=0; int r=anzKacheln-1; int m;
  while(l <= r){
    m = (int) ((r+l)/2);
    if(compare(Element_liste[m][0], Element_liste[m][1], x, y) < 0){
      if(debug2 == 1){printf("<    l=%d, r=%d, m=%d\n",l,r,m );}
      l = m+1;
      continue;
    }
    else if(compare(Element_liste[m][0], Element_liste[m][1], x, y) > 0){
      if(debug2 == 1)printf(">    l=%d, r=%d, m=%d\n",l,r,m );
      r = m-1;
      continue;
    }
    else if(compare(Element_liste[m][0], Element_liste[m][1], x, y) == 0)break;
  }
  //if(binCompare(Element_liste[m][0], Element_liste[m][1], x, y) == 0)return m;
  if(Element_liste[m][0] == x && Element_liste[m][1] == y)return m;

  else{
    //printf("  l=%d, r=%d, m=%d\n",l,r,m );
    return -1;
  }
}

static int compare(unsigned int x1, unsigned y1, unsigned int x2, unsigned int y2){
  if(x1 < x2 ){
      return -1;
  }
  else if(x1 > x2){
      return 1;
  }
  else if(x1 == x2){
    if(y1 < y2){
        return -1;
    }
    else if(y1 > y2){
        return 1;
    }
    else if(y1 == y2)return 0;
  }
}

//-1 wenn a <° b, 1 wenn b >° a, 0 sonst; a <° b :<=> binMeld(A) < binMeld(B)
static int comp(const void* a, const void* b) {
  const unsigned int* A = ( const unsigned int* )a;
  const unsigned int* B = ( const unsigned int* )b;

  return compare(A[0], A[1], B[0], B[1]);
}

void sort(){
   qsort(Element_liste, EL_anz, 2*sizeof(int), comp);
}

//Phase3

void sort_raeume(){
  int raum =0;
  while(R__El_anz[raum] > 0){
    qsort(Raeume[raum], R__El_anz[raum], 2*sizeof(int), comp);
    ++raum;
  }
}

int raeume_linearH_schritt(int index, int i, int gruppiert, int x,int y){

  if(debug3)printf("(%u,%u) Nachbar?\n", Raeume[index][i][0]+x,Raeume[index][i][1]+y);
 	int	k = binSearch(Raeume[index][i][0]+x,Raeume[index][i][1]+y);
  if(k!=-1){
    //schon in Raum eingetragen?
    if(debug3)printf("(%u,%u) Nachbar gruppiert?\n", Raeume[index][i][0]+x,Raeume[index][i][1]+y);

    if(gruppiert_liste[k] == 0){
      if(debug3)printf("(%u,%u) Nachbar eintragen\n", Raeume[index][i][0]+x,Raeume[index][i][1]+y);

      Raeume[index][R__El_anz[index]][0] = Raeume[index][i][0]+x;
      Raeume[index][R__El_anz[index]][1] = Raeume[index][i][1]+y;
      gruppiert_liste[k] = 1;
      return 1;
    }
  }
  return 0;
}

void raeume_linearH(){
  if(anzKacheln % 2 == 1){printf("Keine Loesung moeglich (ungerade Anzahl Kacheln!) %d\n",anzKacheln); exit(0);}
  int index = 0;

  int k=0;
  while(gruppiert < anzKacheln){
    for(; k<anzKacheln; k++){
      if(debug3)printf("Kandidat fuer 1 Element (%d)?\n",k);
      if(gruppiert_liste[k] == 1)continue;
      Raeume[index][0][0] = Element_liste[k][0];
      Raeume[index][0][1] = Element_liste[k][1];
      if(debug3)printf("TESTUS777");
      ++R__El_anz[index];

      gruppiert_liste[k] = 1;
      if(debug3)printf("gruppiert / EL_anz: %u %u\n",gruppiert,anzKacheln);
      if(debug3)printf("gruppe(%d): repraesentant: (%u,%u)\n", index,Raeume[index][0][0], Raeume[index][0][1]);

      ++gruppiert;
      break;
    }

    if(gruppiert == anzKacheln)break;

    int i=0;
    int nachbarn =0;
    while(i < R__El_anz[index]){
      //if(gruppiert % 100 <5)printf("raeume: %d / %d",gruppiert,anzKacheln);

      if(debug3)printf("\n\n Betrachte: (%u,%u)\n",Raeume[index][i][0],Raeume[index][i][1]);

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
      printf("%d hat keinen ungruppierten Nachbarn\n",i );

      if(gruppiert == anzKacheln)break;

      if(debug3)printf("		i=%d / %d Anzahl Element(Raum)\n",i+1,R__El_anz[index]);

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

//Phase4

void greedy(){

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
  if(debug4)printf("waehle: (%u,%u) (gruppiert:%d)\n", Raeume[raum][i][0], Raeume[raum][i][1],index_loesung);

  // Raeume[raum][i][0] = -1;
  // Raeume[raum][i][1] = -1;

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
    if(index_loesung > 0)return 0;
  }//Ende FOR Schleife

  // if(sprung == anzKacheln-1)return 2;
  if(debug4)printf("springe zurueck ...\n");
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

void loesung_binSearch(){
  init();
  einlesen();
  sort();
  raeume_linearH();
  sort_raeume();

  exit(0);

  for(int raum = 0; ;++raum){
    if(R__El_anz[raum] == 0)break;
    printf("loese raum %d\n", raum);
    loesung_prim(raum, Loesung_R, 0,index_loesung);
  }

  printf("\n\nLoesung:\n");
  for(int i=0; i<loesungsindex; i++){
    if(Loesung_liste[i][0] == 0 &&	Loesung_liste[i][1] == 0 && Loesung_liste[i][2] == 0 &&	Loesung_liste[i][3] == 0)continue;
    printf("%u %u;%u %u\n", Loesung_liste[i][0],Loesung_liste[i][1],Loesung_liste[i][2],Loesung_liste[i][3]);
  }

  printf("index_loesung: %u\n",index_loesung);
}

int main(void){loesung_binSearch();}
