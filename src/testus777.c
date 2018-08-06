#include <stdio.h>
#include <stdlib.h>

int *a; int a_platz=1; int a_anz=0;

void a_malloc(){
  a = (int *)malloc(a_platz * sizeof(int));
}

void a_einfuegen(int x){
  if(a_platz == a_anz){
    a_platz*=2;
    printf("size: %d\n",sizeof(a));

    int* newptr = (int *)realloc(a, a_platz*sizeof(int));
    printf("test_size_ptr: %d\n",sizeof(newptr[0]));
    if (newptr == NULL)
    {
        exit(0);
    }
    a = newptr;
    printf("size_ptr: %d\n",sizeof(a));
  }

  a[a_anz] = x;
  ++a_anz;
}

void ausgabe(){
  for(int i=0; i<a_anz; i++){
    printf("%d: %d\n",i,a[i]);
  }
}



int main(void){
  a_malloc();

  for(int i=0; i<100; i++){
    a_einfuegen(i);
  }


  ausgabe();
  printf("size_ptr: %d\n",sizeof(a));
}
