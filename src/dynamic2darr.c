#include <stdio.h>
#include <stdlib.h>

unsigned int **numbers = 0;
int groesse=1000;
int anzahl=0;

static void free_numbers(unsigned int **array, size_t size)
{
    for (size_t i = 0; i < size; i++)
        free(array[i]);
    free(array);
}

static void allozieren(){
  numbers = (unsigned int **)malloc(groesse * sizeof(*numbers));


}

static void reallozieren(){
  int newnum = (groesse + 2) * 2;   /* 4, 12, 28, 60, ... */
  unsigned int **newptr = (unsigned int **)realloc(numbers, newnum * sizeof(*numbers));
  printf("%d\n", groesse);
  if (newptr == NULL)
  {
      free_numbers(numbers, anzahl);
      exit(1);
  }
  groesse = newnum;
  numbers = newptr;
}

static void einfuegen(unsigned int x, unsigned int y){
  if (anzahl == groesse)
  {
    reallozieren();
  }
  numbers[anzahl] = (unsigned int *)malloc(2 * sizeof(unsigned int));
  if (numbers[anzahl] == 0)
  {
      free_numbers(numbers, anzahl);
      exit(1);
  }
  numbers[anzahl][0] = x;
  numbers[anzahl][1] = y;
  anzahl++;
}

static void ausgabe(){
  for(int zeile=0; zeile<anzahl; zeile++){
    printf("%d: (%u,%u)\n",zeile, numbers[zeile][0], numbers[zeile][1]);
  }
}


int main(void)
{
    allozieren();
    einfuegen(2,0);

    for(unsigned int zeile=0; zeile<1000000; zeile++){
        einfuegen(zeile,0);
    }

    ausgabe();

    free_numbers(numbers, anzahl);
    return 0;
}
