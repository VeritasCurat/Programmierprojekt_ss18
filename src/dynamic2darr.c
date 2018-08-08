#include <stdio.h>
#include <stdlib.h>

unsigned int **Element_liste = 0; int EL_platz=1000; int EL_anz=0;

static void EL_free_numbers(unsigned int **array, size_t size)
{
    for (size_t i = 0; i < size; i++)
        free(array[i]);
    free(array);
}

static void EL_allozieren(){
  Element_liste = (unsigned int **)malloc(EL_platz * sizeof(*Element_liste));
}

static void EL_reallozieren(){
  int newnum = (EL_platz + 2) * 2;   /* 4, 12, 28, 60, ... */
  unsigned int **newptr = (unsigned int **)realloc(Element_liste, newnum * sizeof(*Element_liste));
  printf("%d (%d)\n", EL_platz,sizeof(*Element_liste));
  if (newptr == NULL)
  {
      EL_free_numbers(Element_liste, EL_anz);
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
      EL_free_numbers(Element_liste, EL_anz);
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


int main(void)
{
    EL_allozieren();
    EL_einfuegen(2,0);

    int i=0;
    while(1){
        EL_einfuegen(i,0);
        i++;
    }

    EL_ausgabe();

    EL_free_numbers(Element_liste, EL_anz);
    return 0;
}
