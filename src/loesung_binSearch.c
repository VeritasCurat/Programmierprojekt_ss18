/*
 ============================================================================
 Name        : loesung_binSearch.c
 Author      : Johannes Grobelski
 Version     :
 Copyright   : Your copyright noticeo,,,
 Description : Programmierprojekt SS18
 ============================================================================
 */
//TODO: mit devin klaeren: EL_sort() dynamisch;

 #include <string.h> //TODO: entfernen

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <stdint.h>


//Phase1: Initialisieren der DS, Einlesen von String und umwandeln in unsigned int Element_liste[][2]:
  static void EL_reallozieren();
  static void EL_allozieren();
  static void EL_einfuegen(unsigned int x, unsigned int y);
  static void EL_ausgabe();
  static void Array_freigabe(unsigned int **array, size_t size);
  void init();
  void einlesen();
  void koordinate_einlesen(char* zeile);
  int chartoint(char c);
  int legalesZeichen(char c);

//Phase2: Sortieren der Liste mit Quicksort:
  int binSearch(unsigned int x, unsigned int y);
  static int compare(unsigned int x1, unsigned y1, unsigned int x2, unsigned int y2);
  static int comp(const void* a, const void* b);
  void EL_sort();

//Phase3: Raumeinteilung (Raum = Menge von Kachelplaetzen, sodass alle Kachelplaetze min. einen Nachbar aus dieser Menge haben):
  void nl_einfuegen(int x);
  void nl_malloc();
  void nl_ausgabe();
  static void R_sort();
  static void R_reallozieren();
  static void R_allozieren();
  static void R_einfuegen(unsigned int x, unsigned int y, int k);
  static void R_ausgabe();
  static void R_freigabe(unsigned int **array, size_t size);
  static void R_print();
  void sort_raeume();
  int raeume_linearH_schritt(int raumnr,int nachbar_index, int x,int y);
  void raeume_linearH();

//Phase4: Loesung berechnen:
  int loesungsschritt(int** Raum, int raumAnz, unsigned int** loesung, int index_loesung, int x, int y, int i);
  int loesung_prim(unsigned int** Raum, int raumAnz, unsigned int** loesung, int index_loesung);
  void loesung_binSearch();


int debug2 = 0;
int debug3 = 0;
int debug4 = 0;


//Container fuer Vektoren
unsigned int **Element_liste = 0; int EL_platz=1000; int EL_anz=0;//hier werden alle Koordinaten gespeichert
unsigned int **Raeume; int R_anz=0; int R_platz; int gruppiert = 0;
int* nachbar_liste; int nl_anz=0; int nl_platz=1;
unsigned int** raum;  int raum_anz=0; int raum_platz=2;
unsigned int ** loesung; int index_loesung=0;
/*
unsigned int Loesung_R[1000][4]; 	int index_loesung=0; //
unsigned int Loesung_liste[999983][4]; unsigned int loesungsindex = 0;
unsigned int Loesung_Raeume[100][100000][4]; int R__El_anz[100];
*/

//markierung: Kachel in Raum, in loesung?
int* geloest_liste;

size_t liste_size = 999983;

//allozierten speicher freigeben
void beenden(){
  Array_freigabe(Element_liste, EL_anz);
  Array_freigabe(Raeume, R_anz);
}

static void Array_freigabe(unsigned int **array, size_t size){
    for (size_t i = 0; i < size; i++)
        free(array[i]);
    free(array);
}

static void EL_allozieren(){
  Element_liste = (unsigned int **)malloc(EL_platz * sizeof(*Element_liste));
  if(Element_liste == NULL){
    printf("Elementliste konnte nicht alloziert werden!");
    exit(0);
  }
}

static void EL_reallozieren(){
  int newnum = (EL_platz + 2) * 2;   /* 4, 12, 28, 60, ... */
  unsigned int **newptr = (unsigned int **)realloc(Element_liste, newnum * sizeof(*Element_liste));
  printf("%d\n", EL_platz);
  if (newptr == NULL){
      Array_freigabe(Element_liste, EL_anz);
      exit(1);
  }
  EL_platz = newnum;
  Element_liste = newptr;
}

static void EL_einfuegen(unsigned int x, unsigned int y){
  if (EL_anz == EL_platz)
  {
    EL_reallozieren();
  }
  Element_liste[EL_anz] = (unsigned int *)malloc(2 * sizeof(unsigned int));
  if (Element_liste[EL_anz] == 0)
  {
      Array_freigabe(Element_liste, EL_anz);
      exit(1);
  }
  Element_liste[EL_anz][0] = x;
  Element_liste[EL_anz][1] = y;
  EL_anz++;
}

static void EL_ausgabe(){
  for(int zeile=0; zeile<EL_anz; zeile++){
    printf("%d: (%u,%u)\n",zeile, Element_liste[zeile][0], Element_liste[zeile][1]);
  }
}

void init(){
  EL_allozieren();
  nl_malloc();
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
  EL_einfuegen(a,b);
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

//Phase2

//liest unsigned int, gibt 11 stelligen String repraesentant aus
int binSearch(unsigned int x, unsigned int y){
  int l=0; int r=EL_anz-1; int m;
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



/*
int sortiere_0_1 ( const void *pointer_a, const void *pointer_b )
{
    const unsigned int *a = *(const unsigned int **)pointer_a;
    const unsigned int *b = *(const unsigned int **)pointer_b;
    if(a[0] != b[0])
        return a[0] - b[0];
    else
		return a[1] - b[1];

}
*/

void nl_malloc(){
  nachbar_liste = (int *)malloc(nl_platz * sizeof(int));
}

void nl_ausgabe(){
  for(int i=0; i<nl_anz; i++){
    printf("%d: %d\n",i,nachbar_liste[i]);
  }
}

void nl_einfuegen(int x){
  if(nl_anz == nl_platz){
    nl_platz*=2;
    int* newptr = (int *)realloc(nachbar_liste, nl_platz*sizeof(int));
    if (newptr == NULL){
        exit(0);
    }
    nachbar_liste = newptr;
  }

  nachbar_liste[nl_anz] = x;
  ++nl_anz;
}

void EL_sort(){
  unsigned int transfer[EL_anz][2];
  for(int i=0; i<EL_anz; i++){
    transfer[i][0]=Element_liste[i][0];
    transfer[i][1]=Element_liste[i][1];
  }

  qsort(transfer, EL_anz, sizeof(int)*2, comp);
  //qsort(Element_liste, EL_anz, sizeof(Element_liste[0]),comp); falsch
  //qsort(Element_liste, sizeof(Element_liste) / sizeof(*Element_liste), sizeof(Element_liste), comp); falsch

  for(int i=0; i<EL_anz; i++){
    Element_liste[i][0]=transfer[i][0];
    Element_liste[i][1]=transfer[i][1];
  }
}


//Phase3
static int comp_(const void* a, const void* b) {
  const unsigned int* A = ( const unsigned int* )a;
  const unsigned int* B = ( const unsigned int* )b;

  if(A[0] - B[0] < 0)return -1;
  else if(A[0] - B[0] > 0)return 1;
  else return 0;
}

static void R_sort(){
  unsigned int transfer[R_anz][2];
  for(int i=0; i<R_anz; i++){
    transfer[i][0]=Raeume[i][0];
    transfer[i][1]=Raeume[i][1];
  }

  qsort(transfer, R_anz, sizeof(int)*2, comp);

  for(int i=0; i<EL_anz; i++){
    Raeume[i][0]=transfer[i][0];
    Raeume[i][1]=transfer[i][1];
  }
}

static void R_print(){
  for(int i=0; i<gruppiert; i++){
    printf("Raum: %d, Index:%d, (%u,%u)\n",Raeume[i][0], Raeume[i][1], Element_liste[Raeume[i][1]][0],Element_liste[Raeume[i][1]][1]);
  }
}

static void R_einfuegen(unsigned int x, unsigned int y, int k){
  Raeume[k][0] = x;
  Raeume[k][1] = y;
  R_anz++;
}

static void R_allozieren(){
  R_platz = EL_anz;
  Raeume = (unsigned int **)malloc(R_platz * sizeof(*Raeume));
  for(int i=0; i<R_platz; i++){
    Raeume[i] = (unsigned int *)malloc(2 * sizeof(unsigned int));
    if(Raeume[i] == 0){
      Array_freigabe(Raeume, R_anz);
      exit(0);
    }
      Raeume[i][0] =-1;   Raeume[i][1] =-1;
  }

}

static void R_ausgabe(){
  for(int zeile=0; zeile<R_anz; zeile++){
    printf("%d: (%u,%u)\n",zeile, Raeume[zeile][0], Raeume[zeile][1]);
  }
}

int raeume_linearH_schritt(int raumnr, int nachbar_index, int x,int y){
  int k = nachbar_liste[nachbar_index];

  if(debug3)printf("(%u,%u) Nachbar?\n", Element_liste[k][0]+x,Element_liste[k][1]+y);
 	int	k_nachbar = binSearch(Element_liste[k][0]+x,Element_liste[k][1]+y);
  if(k_nachbar!=-1){
    //schon in Raum eingetragen?
    if(debug3)printf("(%u,%u) Nachbar gruppiert?\n", Element_liste[k][0]+x,Element_liste[k][1]+y);

    if(Raeume[k_nachbar][0] == -1){
      if(debug3)printf("(%u,%u) Nachbar eintragen\n", Element_liste[k][0]+x,Element_liste[k][1]+y);

      R_einfuegen(raumnr, k_nachbar, k_nachbar);
      Raeume[k_nachbar][0] = raumnr;
      Raeume[k_nachbar][1] = k_nachbar;

      nl_einfuegen(k_nachbar);
      //nl_ausgabe();
      return 1;
    }
  }
  return 0;
}

void raeume_linearH(){
  R_allozieren();

  if(EL_anz % 2 == 1){printf("Keine Loesung moeglich (ungerade Anzahl Kacheln!) %d\n",EL_anz); exit(0);}
  int raumnr = 0;

  int k=0;

  while(gruppiert < R_platz){
    for(; k<EL_anz; ++k){
      if(k==EL_anz-1 && gruppiert < R_platz)k=0;
      if(debug3)printf("Kandidat fuer 1 Element (%d)?\n",k);
      if(Raeume[k][0] != -1)continue;
      R_einfuegen(raumnr, k, k);

      if(debug3)printf("gruppiert / EL_anz: %u %u\n",gruppiert,EL_anz);
      if(debug3)printf("gruppe(%d): repraesentant: (%u,%u)\n", raumnr,Element_liste[k][0], Element_liste[k][1]);

      ++gruppiert;
      break;
    }

    Raeume[k][0] = raumnr;
    Raeume[k][1] = k;


    nachbar_liste[0] = k;
    ++nl_anz;


    if(gruppiert == R_platz)break;

    int nachbar_index =0;
    while(nachbar_index < nl_anz){
      if(debug3)printf("\n\n Betrachte: (%u,%u)\n",Element_liste[nachbar_liste[nachbar_index]][0], Element_liste[nachbar_liste[nachbar_index]][1]);

      //linker Nachbar?
      if(raeume_linearH_schritt(raumnr, nachbar_index, 0, 1) == 1){
        ++gruppiert;
      }
      if(nachbar_index == nl_anz)break;
      //rechter Nachbar?
      if(raeume_linearH_schritt(raumnr, nachbar_index, 0, -1) == 1){
        ++gruppiert;
      }
      if(nachbar_index == nl_anz)break;
      //oberer Nachbar?
      if(raeume_linearH_schritt(raumnr,  nachbar_index, 1, 0) == 1){
        ++gruppiert;
      }
      if(nachbar_index == nl_anz)break;
      //unterer Nachbar?
      if(raeume_linearH_schritt(raumnr, nachbar_index, -1, 0) == 1){
        ++gruppiert;

      }
      if(nachbar_index == nl_anz)break;

      if(debug3)printf("		i=%d / %d Anzahl Element(Raum)\n",nachbar_index,nl_anz);

      ++k;
      ++nachbar_index;
    }
    if(gruppiert == EL_anz)break;

    if(nl_anz % 2 == 1){
      printf("Ein Raum hat eine ungerade Anzahl von Kacheln (Raum %d: %d Kacheln)\n",raumnr, nl_anz);
      exit(-1);
    }
    for(int i=0; i<nl_anz; i++){
      nachbar_liste[i] = -1;
    }
    nl_anz = 0;

    //sortiere (notwendig?)
    //EL_sort();
    ++raumnr;
  }
}

//Phase4

void greedy(){

}

void r_loeschen(){
  int newnum = 1;   /* 4, 12, 28, 60, ... */
  unsigned int **newptr = (unsigned int **)realloc(raum, newnum * sizeof(*raum));
  if (newptr == NULL){
      for (size_t i = 0; i < raum_anz; i++)free(raum[i]);
      free(raum);
      beenden();
      exit(1);
  }
  raum_platz = newnum;
  raum = newptr;
  raum_anz = 0;
}

void r_einfuegen(unsigned int x, unsigned y){
  if(raum_platz == raum_anz){
    //printf("%d ~ %d\n", raum_platz, raum);
    int newnum = (raum_platz * 2)+2;   /* 4, 12, 28, 60, ... */
    unsigned int **newptr = (unsigned int **)realloc(raum, newnum * sizeof(*raum));
    if (newptr == NULL){
        for (size_t i = 0; i < raum_anz; i++)free(raum[i]);
        free(raum);
        beenden();
        exit(1);
    }
    raum_platz = newnum;
    raum = newptr;
  }

  raum[raum_anz] = (unsigned int *)malloc(2 * sizeof(unsigned int));

  raum[raum_anz][0] = x;
  raum[raum_anz][1] = y;
  ++raum_anz;
}

void r_allozieren(){
  raum = (unsigned int **)malloc(2 * sizeof(*raum));
}

int loesung_prim(int** Raum, int raumAnz, unsigned int** loesung, int index_loesung){
  //printf("raum: %d, %d/ %d  Elemente geloest \n",raum,index_loesung,R__El_anz[raum]/2);
  //gueltige loesung: basisfall
  if(raumAnz == 0){
    return 1;
  }

  int suche=0;

  int i;
  for(i=0; i<raumAnz; i++){
    //1. nehme erste verbleibende in Element_liste
    //pruefe ob kachel aus Element_liste schon in loesung
    if(index_loesung[i] == 0){
    }

    int geloest;
    geloest = geloest_liste[i];
    if(geloest == 1){
      continue;
    }
    if(debug4)printf("waehle: (%u,%u) (gruppiert:%d)\n",  Element_liste[i][0],Element_liste[i][1],index_loesung);

     //2. suche Nachbarn
    //oberer Nachbarn?
      int ret =0;
      ret = loesungsschritt(Raum, raumAnz,loesung, index_loesung, 0 , 1,i);
      if(ret == 1)return 1;

    //unterer Nachbarn?
      ret = loesungsschritt(Raum, raumAnz,loesung, index_loesung, 0 , -1,i);
      if(ret == 1)return 1;

    //rechter Nachbarn?
      ret = loesungsschritt(Raum, raumAnz,loesung, index_loesung, 1, 0,i);
      if(ret == 1)return 1;

    //linker Nachbarn?
      ret = loesungsschritt(Raum, raumAnz,loesung, index_loesung, -1 , 0,i);
      if(ret == 1) return 1;

    //printf("keinen nachbar fuer (%u,%u) (gruppiert: %d)\n",Raeume[raum][i][0], Raeume[raum][i][1],index_loesung);
    if(index_loesung > 0)return 0;
  }//Ende FOR Schleife

  // if(sprung == EL_anz-1)return 2;
  if(debug4)printf("springe zurueck ...\n");
  return 2;
}

int loesungsschritt(int** Raum, int raumAnz, unsigned int** loesung, int index_loesung, int x, int y, int i){
  //existiert nachbar
  int nachbar_index = binSearch(Element_liste[i][0]+x, Element_liste[i][1]+y);
  if(nachbar_index != -1){
    //pruefe ob kachel aus Element_liste schon in loesung
    int geloest = geloest_liste[nachbar_index];
    if(geloest == 0){
      //printf("%d%d		neue Nachbarn: (%u,%u) (%u,%u) (gruppiert: %d)\n",x,y, Raeume[raum][i][0], Raeume[raum][i][1], Raeume[raum][i][0]+x, Raeume[raum][i][1]+y,index_loesung);
      geloest_liste[i] = 1;
      geloest_liste[nachbar_index]=1;

      loesung[index_loesung][0] = Element_liste[i][0];
      loesung[index_loesung][1] = Element_liste[i][1];
      loesung[index_loesung][2] = Element_liste[nachbar_index][0];
      loesung[index_loesung][3] = Element_liste[nachbar_index][1];

      ++index_loesung;
      raumAnz-=2;

      int l = loesung_prim(Raum, raumAnz,  loesung, index_loesung);
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
        raumAnz+=2;

        //printf("	 Nachbarn wurden geloescht ...\n");
      }
    }
  }


  return 2;
}

void loesung_binSearch(){
  init();
  einlesen();
  EL_sort();
  //EL_ausgabe();
  raeume_linearH();
  R_sort();
  //R_print();

  geloest_liste = (int*) malloc(EL_anz*sizeof(int));
  for(int i=0; i<EL_anz; i++)geloest_liste[i] = 0;

  loesung = (unsigned int**) malloc((EL_anz/2)*sizeof(int)*4);
  for(int i=0; i<EL_anz/2; i++){
    loesung[i] = (unsigned int *)malloc(4 * sizeof(unsigned int));
    loesung[i][0] = loesung[i][1] = loesung[i][2] = loesung[i][3] = 0;
  }

  r_allozieren();

  int raumnr=0;

  for(int i=0; i<gruppiert; ++i){
    if(i < gruppiert && Raeume[i][0] > raumnr){
      loesung_prim(raum, raum_anz, loesung, index_loesung);
      //for(int j=0; j<raum_anz; j++)printf("raum %d: %d (%u,%u)\n",raumnr,j,raum[j][0],raum[j][1]);
      raumnr = Raeume[i][0];
      r_loeschen();
      printf("neuer Raum: %d\n", Raeume[i+1][0]);
    }

    r_einfuegen(Element_liste[Raeume[i][1]][0],Element_liste[Raeume[i][1]][1]);
    if(i == gruppiert-1){
      loesung_prim(raum, raum_anz, loesung, index_loesung);
      //for(int j=0; j<raum_anz; j++)printf("raum %d: %d (%u,%u)\n",raumnr,j,raum[j][0],raum[j][1]);
      raumnr = Raeume[i][0];
      r_loeschen();
    }
  }



  exit(0);

  printf("\n\nLoesung:\n");
  for(int i=0; i<index_loesung; i++){
    if(loesung[i][0] == 0 &&	loesung[i][1] == 0 && loesung[i][2] == 0 &&	loesung[i][3] == 0)continue;
    printf("%u %u;%u %u\n", loesung[i][0],loesung[i][1],loesung[i][2],loesung[i][3]);
  }

  printf("index_loesung: %u\n",index_loesung);
}

int main(void){
  loesung_binSearch();
  exit(0);


  beenden();
}
