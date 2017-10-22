void* pamat;
#include <stdio.h>
#include <string.h>

#define debug 1

void set(unsigned int poz, signed int hodnota) {
	*(int *) (pamat + poz) = hodnota;
}

int get(unsigned int poz) {
	return *(int *) (pamat + poz);
}

void flip(unsigned int poz) {
	set(poz, -get(poz));
}

void vypis() {
	int i, size = *(int *) pamat;
	
	size = size - (size % sizeof(int));

	for (i = 0; i < size; i += sizeof(int))
		printf("%d:\t%d\n", i, get(i));
}

void *memory_alloc(unsigned int size) {
	unsigned int pov_hlavicka = get(sizeof(int));
	
	// Ak v danom poli neexistuje volny blok
	if (pov_hlavicka < 0) { return NULL; }
	
	int next = get(pov_hlavicka + sizeof(int));
	int prev = get(pov_hlavicka + 2 * sizeof(int));
	int pov_dlzka = get(pov_hlavicka);
	unsigned int pov_paticka = pov_hlavicka - pov_dlzka + sizeof(int);
	
	if (debug) {
		printf("memory_alloc Debug:\n");
		printf("size: %u\n", size);
	}
	
	// Ak prvy volny blok je dostatocne velky a da sa rozdelit
	if (((signed int) (size + pov_dlzka) < 0) && (pov_dlzka >= 6 * sizeof(int))) {
		unsigned int hlavicka = pov_hlavicka;
		unsigned int paticka, dlzka;
		
		if ((size % 4) == 0) {
			paticka = hlavicka + size + sizeof(int);
			dlzka = size;
		}
		// 4 Byte Padding
		else {
			int pom = size / 4;
			pom = (pom + 1) * 4;
			paticka = hlavicka + pom + sizeof(int);
			dlzka = pom;
		}
		
		if (debug) {
			printf("pov_dlzka: %d\n", pov_dlzka);
			printf("pov_paticka: %d\n", pov_paticka);
			printf("hlavicka: %u\n", hlavicka);
			printf("dlzka: %u\n", dlzka);
			printf("paticka: %u\n", paticka);
		}
		
		int nova_dlzka = pov_dlzka + dlzka + 2 * sizeof(int);
		
		set(hlavicka, dlzka);
		set(paticka, dlzka);
		set(paticka + sizeof(int), nova_dlzka);
		set(pov_paticka, nova_dlzka);
		set(sizeof(int), paticka + sizeof(int));
		set(paticka + 2 * sizeof(int), next);
		set(paticka + 3 * sizeof(int), prev);
		
		return (pamat + hlavicka + sizeof(int));
	}
	// Ak prvy volny blok je prave rovnako velky alebo nerozdelitelny
	else if ((signed int) (size + pov_dlzka) <= 0) {
		flip(pov_hlavicka);
		flip(pov_paticka);
		set(sizeof(int), -1);
		
		if (next > 0) {
			set(next + 2 * sizeof(int), prev);
		}
		
		return (pamat + pov_hlavicka + sizeof(int));
	}
	// Ak prvy volny blok nie je dostatocne velky
	else {
		// TODO
	}
	
	return NULL;
}

int memory_free(void *valid_ptr) {
	int hlavicka = valid_ptr - pamat - sizeof(int);
	int dlzka = get(hlavicka);
	
	if (debug) {
		printf("memory_free Debug:\n");
		printf("hlavicka: %d\n", hlavicka);
		printf("dlzka: %d\n", dlzka);
	}
	
	// Zmena hodnoty hlavicky na zapornu
	flip(hlavicka);
	
	// Zmena hodnoty paticky na zapornu
	flip(hlavicka + dlzka + sizeof(int));
	
	// Ak je toto jediny volny blok
	if (get(sizeof(int)) < 0) {
		set(sizeof(int), hlavicka);
		set(valid_ptr - pamat, - 1);
		set(valid_ptr - pamat + sizeof(int), -1);
	}
	
	// TODO Najst predchadzajuci volny blok
	
	// TODO Najst nasledujuci volny blok
	
	return 0;
}

// Ak je smernik platny vrati 1, ak neplatny vrati 0
int memory_check(void *ptr) {
	if (debug) {
		printf("memory_check Debug:\n");
	}
	
	// Kontrola ci je dana pamat v rozsahu
	if (ptr > (pamat + *(int *)pamat)) { return 0; }
	
	int hlavicka = get(ptr - pamat - sizeof(int));
	
	if (debug) {
		printf("hlavicka: %d\n", hlavicka);
	}
	
	if (hlavicka < 0) { return 0; }
	
	int paticka = get(ptr - pamat + hlavicka);
	
	if (debug) {
		printf("paticka: %d\n", paticka);
	}
		
	if ((paticka < 0) || (hlavicka != paticka)) { return 0; }
	
	return 1;
}

void memory_init(void *ptr, unsigned int size) {
	int i;
	
	size = size - (size % sizeof(int));
	
	pamat = ptr;
	set(0, size);
	set(sizeof(int), 2 * sizeof(int));
	set(2 * sizeof(int), -(size - 4 * sizeof(int)));
	set(3 * sizeof(int), -1);
	set(4 * sizeof(int), -1);
	set(get(0) - sizeof(int), -(size - 4 * sizeof(int)));
	
	for (i = 5 * sizeof(int); i < size - sizeof(int); i += sizeof(int)) {
		set(i, 0);
	}
	
	if (debug) {
		printf("memory_init Debug:\n");
		vypis();
	}
	
	// Priprava na hlavicky variabilnej velkosti
	/*if (size <= SCHAR_MAX) {
		printf("Hlavicky velkosti 1B\n");
		*(char *) pamat = 1;
		*(signed char *) (pamat + 1) = (signed char) size;
	}
	else if (size <= SHRT_MAX) {
		printf("Hlavicky velkosti 2B\n");
		*(char *) pamat = 2;
		*(short *) (pamat + 2) = (short) size;
	}
	else if (size <= INT_MAX) {
		printf("Hlavicky velkosti 4B\n");
		*(char *) pamat = 4;
		*(int *) (pamat + 4) = (int) size;
	}
	else {
		printf("Prilis velke pole!\n");
		printf("Pre viac informacii kontaktuje Vasho systemoveho administratora.\n");
	}*/
}

int main() {
  char region[64];
  memory_init(region, 64);
  printf("Main dostal z memory_alloc: %d\n", memory_alloc(48) - pamat);
  /*
  printf("Memory check 12: %d\n", memory_check(region + 12));
  printf("Memory free 12: %d\n", memory_free(region + 12));
  printf("Memory check 12: %d\n", memory_check(region + 12));
  */
  getchar();
  vypis();
  /*char* pointer = (char*) memory_alloc(10);
  if (pointer)
    memset(pointer, 0, 10);
  if (pointer)
    memory_free(pointer);*/
  return 0;
}
