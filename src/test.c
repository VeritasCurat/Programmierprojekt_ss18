#include "loesung_binSearchALT.c"
//Funktionen des Algorithmus:
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

  //Phase3: Raumeinteilung (Raum = Menge von Kachelplätzen, sodass alle die einen Nachbar aus Menge haben):
    void sort_raeume();
    int raeume_linearH_schritt(int index, int i, int gruppiert, int x,int y);
    void raeume_linearH();

  //Phase4: Loesung berechnen:
    int loesungsschritt(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung, int x, int y, int i);
    int loesung_prim(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung);
    void loesung_binSearch();



//Testfunktionen
  //Phase1: Einlesen
  void test_eingabe();

  //Phase2: Sortieren
  void test_sort();

  //Phase3: Raumeinteilung
  int validate_raeume();
  void binSearchTEST();
  int lineare_suche(unsigned int x, unsigned int y);
  int abs(int a);
  int benachbart(unsigned int x[], unsigned int y[]);
  int validate_raeume();

  //Phase4: Lösen
  int validate();
  int validate2();

//Phase 2:
void test_sort(){
  printf("Sortierung ...");
  for(int i=0; i<anzKacheln-1; i++){
    if(compare(Element_liste[i][0], Element_liste[i][1],Element_liste[i+1][0], Element_liste[i+1][1]) > 0){
      printf("FEHLER in sortierung: %d,%d \n",i,i+1);
      printf("%u,%u > %u,%u \n",Element_liste[i][0], Element_liste[i][1],Element_liste[i+1][0], Element_liste[i+1][1]);
    }
  }
  printf(" OK!\n");
}

//Phase 3:
void binSearchTEST(){
  printf("binSearch TEST ...");
  int fehlgeschlagen=0;
  for(int i=0; i<anzKacheln; i++){
    //printf("(%u,%u): \n",Element_liste[i][0],Element_liste[i][1]);

    if(binSearch(Element_liste[i][0],Element_liste[i][1]) == -1){
      fehlgeschlagen = 1;
      debug2 = 1;
      binSearch(Element_liste[i][0],Element_liste[i][1]);
      debug2 = 0;
      printf("lineare Suche: %d \n",lineare_suche(Element_liste[i][0],Element_liste[i][1]));
      printf("nicht gefunden!\n\n\n");
    }
    else {
      continue;
      //printf("gefunden!\n");
    }
  }
  if(fehlgeschlagen == 0)printf("%s\n", " OK!" );
}

int lineare_suche(unsigned int x, unsigned int y){
  for(int i=0; i< anzKacheln; i++){
    if(Element_liste[i][0] == x && Element_liste[i][1] == y){
      return i;

    }
  }
  return -1;
}

int abs(int a){
  if(a < 0) return a*-1;
  return a;
}

int benachbart(unsigned int x[], unsigned int y[]){
  if(abs((x[0] - y[0]) + (x[1] - y[1])) == 1) return 1;
  return 0;
}

int validate_raeume(){
  printf("Raumeinteilung ...");
  int valid=1;
  //ist jedes Element aus Raemen in Elementliste?
  int anzraumkacheln = 0;
  int raum = 0;

  while(R__El_anz[raum] != 0){
    for(int i=0; i<R__El_anz[raum]; i++){
      if(Raeume[raum][i][0] != -1 && Raeume[raum][i][0] != -1){
      int k;
        if(binSearch(Raeume[raum][i][0],Raeume[raum][i][1]) == -1){
          printf("Element %d aus Raum %d existiert nicht!\n",i,raum);
        }
      }
    }
    ++raum;
  }
  if(valid == 0)return valid;

  //ist jedes Element aus Elementliste in Raum?
  if(gruppiert < anzKacheln){
    for(int i=0; i<anzKacheln; i++){
      if(gruppiert_liste[i] == 0){
        printf("Element %d (%u,%u) nicht gruppiert!\n",i,Element_liste[i][0],Element_liste[i][1]);
      }
    }
  }
  if(valid == 0)return valid;


  //genügt jeder Raum den Raumeigenschaften (benachbart Eigenschaften)
  int raumCHECK =0;
  raum = 0;
  while(R__El_anz[raum] > 0){
  for(int i=0; i<R__El_anz[raum]; ++i){
    //linker Nachbar?
      if(benachbart(Raeume[raum][i],Raeume[raum][i-1]) == 1){
        continue;
      }
    //rechter Nachbar?
      if(benachbart(Raeume[raum][i],Raeume[raum][i-1]) == 1){
        continue;
      }
    //oberer Nachbar?
      int k1=i;
      int signal = 0;
      while(k1 > 0){
        if(benachbart(Raeume[raum][i],Raeume[raum][k1]) == 1) {signal = 1; break;}
        if(abs(Raeume[raum][k1][0] - Raeume[raum][i][0]) > 1)break;
        --k1;
      }
      if(signal == 1)continue;
    //unterer Nachbar?
      int k2 = i;
      signal = 0;
      while(k2 < anzKacheln){
        if(benachbart(Raeume[raum][i],Raeume[raum][k2]) == 1) {signal = 1; break;}
        if(abs(Raeume[raum][k2][0] - Raeume[raum][i][0]) > 2)break;
        ++k2;
      }
      if(signal == 1)continue;

      valid = 0;
      printf("Element %d (%u,%u) hat keinen Nachbarn!\n",i,Raeume[raum][i][0],Raeume[raum][i][1]);
      //printf("    k1 = %d, k2 = %d\n",k1,k2);

    }
    ++raum;


    // for(int j=0; j<R__El_anz[raum]; ++j){
    //       if(benachbart(Raeume[raum][i],Raeume[raum][j]) == 1){
    //         raumCHECK+=(i+1);
    //         // printf("%d benachbart: (%u,%u)(%u,%u)\n",i,Raeume[raum][i][0],Raeume[raum][i][1],Raeume[raum][j][0],Raeume[raum][j][1] );
    //         break;
    //       }
    // }


    // if(raumCHECK != ((R__El_anz[raum] * (R__El_anz[raum]+1))/2)){
    //   printf("Raum %i nicht zusammenhaengend %u != %u !\n",raum,raumCHECK,((R__El_anz[raum] * (R__El_anz[raum]+1))/2));
    //   valid = 0;
    // }
    // ++raum;
    // raumCHECK = 0;
    // }
  }
  if(valid == 1)printf(" OK!\n");
  else printf(" nicht OK!\n");

  return valid;
}

//Phase 4:
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

void printraeume(){
  int index =0;
  while(R__El_anz[index] != 0){
    printf("Raum %d (%d):\n",index,R__El_anz[index]);
    for(int i=0; i<R__El_anz[index]; i++){
      printf("%d: x: %u, y: %u\n",i, Raeume[index][i][0], Raeume[index][i][1]);
    }
  ++index;
  }
}

void printlist(){
  for(int i=0; i<anzKacheln; i++){
    printf("%u: A: %u, B: %u\n", i, Element_liste[i][0] , Element_liste[i][1]);
  }
}

int testus(void){
    einlesen();
    sort();
    test_sort();
    binSearchTEST();
    raeume_linearH();
    sort_raeume();
    printraeume();
    validate_raeume();
    /*


    */
}

void main(){testus();}
