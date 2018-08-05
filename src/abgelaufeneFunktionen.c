
void loesung_ober(int raum, unsigned int loesung[][4], int index_loesung_raum, int index_loesung){
	int sprung = 0;
	while(1){
		for(int i=0; i<500000; i++){
			if(Hash_geloest[i][0] != 0 || Hash_geloest[i][1] != 0){
				//printf("hash nicht leer (%d,%d)!!!\n", Hash_geloest[i][0], Hash_geloest[i][1]);
				//exit(0);
				Hash_geloest[i][0] = 0; Hash_geloest[i][1] = 0;
			}
		}
		printf("\n\n\nEbene 0, Sprung: %d", sprung);
		if(sprung == R__El_anz[raum]-1)break;
		if(loesung_prim(raum, loesung, 0,index_loesung,sprung) == 1)break;
		++sprung;
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
}

//bijektive Abbildung "Konkatenation": unsigned int X unsigned int => String, (x,y) -> x°y
void binMeld(char res[22], unsigned int x, unsigned int y){
  if(debug) printf("      meld: (%u, %u)\n",x,y);
  char a[11]="";
  char b[11]="";

  inttostring(a,x);
  inttostring(b,y);

  if(debug) printf("x = %s\n",a);
  if(debug) printf("y = %s\n",b);

  int cnt =0;
  for(int i=0; i<10; i++){
    if(a[i] != '\0'){res[cnt] = a[i];++cnt;}
  }
  for(int i=0; i<10; i++){
    if(b[i] != '\0'){res[cnt] = b[i];++cnt;}
  }

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
