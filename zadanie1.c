// zadanie1.c -- Zadanie 1 - Spravca pamati
// Stanislav Jakubek, 5.10.2015 09:37:23

void* pamat;
#include <stdio.h>

void *memory_alloc(unsigned int size)
{
  int akt = sizeof(int);
  
  while ((*(int *) (pamat + akt) > 0 ) &&
         (*(int *) (pamat + akt) + size > 0) &&
         ((pamat + akt) < (pamat + *(int *)pamat))) {
    //najdenie dostatocne velkej volnej pamate
    akt += *(int *) (pamat + akt) + sizeof(int);
  }
  
  if ((int) ((*(int *) (pamat + akt)) + size) <= 0) {
    // ak sa do tejto pamati zmesti
    if (-*(int *) (pamat + akt) > (size + sizeof(int))) {
      // pamat[akt] patri do intervalu (size+sizeof(int); ∞)
      // zmeni cislo v sucasnej hlavicke a vytvori dalsiu
      *(int *) (pamat + akt + size + sizeof(int)) =
        (signed int) ((*(int *) (pamat + akt)) + size + sizeof(int));
      *(int *) (pamat + akt) = size;
      return (pamat + akt + sizeof(int));
    }
    else if ((-*(int *) (pamat + akt) <= (size + sizeof(int))) &&
             (-*(int *) (pamat + akt) >= size)) {
      // pamat[akt] patri do intervalu <size; size+sizeof(int)>
      // ponecha cislo v sucasnej hlavicke
      *(int *) (pamat + akt) = -*(int *) (pamat + akt);
      return (pamat + akt + sizeof(int));
    }
  }
  else {
    // ak sa do tejto pamati nezmesti
    return NULL;
  }
  return NULL;
}

int memory_free(void *valid_ptr)
{ 
  // jednoducho zmeni hodnotu hlavicky z kladnej na zapornu
  *(int *) (valid_ptr - sizeof(int)) = -*(int *) (valid_ptr - sizeof(int));
  
  //nefunkcny pokus o vyriesenie fragmentacie
  /*int akt = sizeof(int);
  // prejdenie celej pamate a pospajanie volnych pamati
  while (((pamat + akt) <= (pamat + *(int *)pamat - 1)) &&
         ((pamat + akt + *(int *)(pamat + akt) + sizeof(int)) <= (pamat + *(int *)pamat - 1))) {
    // ak je pamat[akt] a aj nasledujuca cast pamateplatna - porovnavanie adries
    while ((*(int *) (pamat + akt) < 0) &&
           (*(int *) (pamat + akt + (*(int *)(pamat + akt)) + sizeof(int)) < 0)) {
      // pokial je pamat[akt] < 0 a aj nasledujuca cast pamate < 0
      *(int *) (pamat + akt) +=
        -(*(int *) (pamat + akt + *(int *)(pamat + akt) + sizeof(int))) + sizeof(int);
    }
    akt += *(int *) (pamat + akt) + sizeof(int);
  }*/
  
  return 0;
}

int memory_check(void *ptr)
{
  int akt = sizeof(int);
  
  while (((pamat + akt + sizeof(int)) != ptr) &&
         ((pamat + akt) < (pamat + *(int *)pamat))) {
    //najdenie zadaneho smernika
    if (*(int *) (pamat + akt) > 0) {
      akt += *(int *) (pamat + akt) + sizeof(int);
    }
    else if (*(int *) (pamat + akt) < 0) {
      akt += -(*(int *) (pamat + akt)) + sizeof(int);
    }
  }
  
  if ((pamat + akt) < (pamat + *(int *)pamat) &&
      ((pamat + akt + sizeof(int)) == ptr) &&
      (*(int *)(pamat + akt) < 0)) {
    //ak je smernik platny
    return 1;
  }
  else {
    //ak je smernik neplatny
    return 0;
  }
  return 0;
}

void memory_init(void *ptr, unsigned int size)
{
  pamat = ptr;
  *(int *) pamat = (unsigned int) size;
  *(int *) (pamat + sizeof(int)) = (signed int) -(size - 2 * sizeof(int));
  //Kontrolny vypis
  //printf("SIZE: %d PAMAT0: %d PAMAT1: %d\n", size, *(int *) pamat, *(int *) (pamat + sizeof(int)));
}

// Tlacidlo 'Spustit' vyskusa vasho spravcu pamati na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.
