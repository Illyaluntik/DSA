void* pamat;
#include <stdio.h>
#include <string.h>

#define debug 0

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

// Vrati smernik na alokovany kus pamate, inak NULL
void *memory_alloc(unsigned int size) {
	unsigned int pov_hlavicka = get(sizeof(int));

	if (debug) {
		printf("memory_alloc Debug:\n");
		printf("size: %u\n", size);
		printf("pov_hlavicka: %u\n", pov_hlavicka);
	}
	
	// Ak v danom poli neexistuje volny blok
	if (pov_hlavicka == 0) { return NULL; }

	int next = get(pov_hlavicka + sizeof(int));
	int prev = get(pov_hlavicka + 2 * sizeof(int));
	int pov_dlzka = - get(pov_hlavicka);
	unsigned int pov_paticka = pov_hlavicka + pov_dlzka + sizeof(int);
	
	if (debug) {
		printf("pov_dlzka: %d\n", pov_dlzka);
		printf("pov_paticka: %u\n", pov_paticka);
	}
	
	// Ak prvy volny blok je dostatocne velky a da sa rozdelit
	if (((signed int) (size - pov_dlzka) < 0) &&
		(pov_dlzka >= 6 * sizeof(int)) && ((pov_dlzka - size) >= 4 * sizeof(int))) {
		unsigned int hlavicka = pov_hlavicka;
		unsigned int paticka, dlzka;
		
		if ((size % sizeof(int)) == 0) {
			paticka = hlavicka + size + sizeof(int);
			dlzka = size;
		}
		// Sizeof(int) Padding
		else {
			int pom = size / sizeof(int);
			pom = (pom + 1) * sizeof(int);
			paticka = hlavicka + pom + sizeof(int);
			dlzka = pom;
		}
		
		if (debug) {
			printf("hlavicka: %u\n", hlavicka);
			printf("dlzka: %u\n", dlzka);
			printf("paticka: %u\n", paticka);
			printf("next: %d\n", next);
		}
		
		int nova_dlzka = - (pov_dlzka - dlzka - 2 * sizeof(int));
		
		set(hlavicka, dlzka);
		set(paticka, dlzka);
		set(paticka + sizeof(int), nova_dlzka);
		set(pov_paticka, nova_dlzka);
		set(sizeof(int), paticka + sizeof(int));
		set(paticka + 2 * sizeof(int), next);
		set(paticka + 3 * sizeof(int), prev);

		if (next > 0) {
			set(next + 2 * sizeof(int), paticka + sizeof(int));
		}
		
		if (debug) { vypis(); }
		return (pamat + hlavicka + sizeof(int));
	}
	// Ak prvy volny blok je prave rovnako velky alebo nerozdelitelny
	else if ((signed int) (size - pov_dlzka) <= 0) {
		flip(pov_hlavicka);
		flip(pov_paticka);
		set(sizeof(int), 0);
		
		if (next > 0) {
			set(sizeof(int), next);
			set(next + 2 * sizeof(int), -1);
		}
		
		if (debug) { vypis(); }
		return (pamat + pov_hlavicka + sizeof(int));
	}
	// Ak prvy volny blok nie je dostatocne velky
	else {
		// TODO
		if (debug) {
			printf("Prvy volny blok nie je dostatocne velky\n");
		}
	}
	
	if (debug) { vypis(); }
	return NULL;
}

// Ak sa podarilo uvolnit pamat vrati 0, inak vrati 1
int memory_free(void *valid_ptr) {
	unsigned int hlavicka = valid_ptr - pamat - sizeof(int);
	unsigned int dlzka = get(hlavicka);
	unsigned int paticka = hlavicka + dlzka + sizeof(int);
	
	if (debug) {
		printf("memory_free Debug:\n");
		printf("hlavicka: %u\n", hlavicka);
		printf("dlzka: %u\n", dlzka);
		printf("paticka: %u\n", paticka);
	}
	
	// Zmena hodnoty hlavicky na zapornu
	flip(hlavicka);
	
	// Zmena hodnoty paticky na zapornu
	flip(paticka);
	
	// Ak je toto jediny volny blok
	if (get(sizeof(int)) == 0) {
		set(sizeof(int), hlavicka);
		set(valid_ptr - pamat, - 1);
		set(valid_ptr - pamat + sizeof(int), -1);
		
		if (debug) {
			printf("Je to jediny volny blok\n");
			vypis();
		}
		
		return 0;
	}
		
	unsigned int next_hlavicka = paticka + sizeof(int);
	unsigned int prev_paticka = hlavicka - sizeof(int);
	
	if (debug) {
		printf("Nie je to jediny volny blok\n");
		printf("next_hlavicka: %u\n", next_hlavicka);
		printf("prev_paticka: %u\n", prev_paticka);
	}
	
	// Ak je uvolneny blok obkoleseny volnymi blokmi
	if ((next_hlavicka < get(0)) && (get(next_hlavicka) < 0) &&
		(prev_paticka > sizeof(int)) && (get(prev_paticka) < 0)) {
		unsigned int next_dlzka = - get(next_hlavicka);
		unsigned int next_paticka = next_hlavicka + next_dlzka + sizeof(int);
		unsigned int prev_dlzka = - get(prev_paticka);
		unsigned int prev_hlavicka = prev_paticka - prev_dlzka - sizeof(int);
		int next_next = get(next_hlavicka + sizeof(int));
		unsigned int new_dlzka = prev_dlzka + dlzka + next_dlzka + 4 * sizeof(int);
		
		if (debug) {
			printf("Uvolnovany blok je obkoleseny volnymi blokmi\n");
			printf("prev_dlzka: %u\n", prev_dlzka);
			printf("prev_paticka: %u\n", prev_paticka);
			printf("next_dlzka: %u\n", next_dlzka);
			printf("next_paticka: %u\n", next_paticka);
			printf("next_next: %d\n", next_next);
			printf("new_dlzka: %u\n", dlzka);
		}

		set(prev_hlavicka, - new_dlzka);
		set(next_paticka, - new_dlzka);
		set(prev_hlavicka + sizeof(int), next_next);
		
		if (next_next > 0) {
			set(next_next + 2 * sizeof(int), prev_hlavicka);
		}

		if (debug) { vypis(); }
		return 0;
	}
	// Ak za uvolnenym blokom nasleduje dalsi volny blok
	else if ((next_hlavicka < get(0)) && (get(next_hlavicka) < 0)) {
		unsigned int next_dlzka = - get(next_hlavicka);
		unsigned int next_paticka = next_hlavicka + next_dlzka + sizeof(int);
		int next_next = get(next_hlavicka + sizeof(int));
		int next_prev = get(next_hlavicka + 2 * sizeof(int));
		
		if (debug) {
			printf("Za uvolnovanym blokom nasleduje dalsi volny\n");
			printf("next_dlzka: %u\n", next_dlzka);
			printf("next_paticka: %u\n", next_paticka);
			printf("next_next: %d\n", next_next);
			printf("next_prev: %d\n", next_prev);
		}
		
		// Nastavenie novych "smernikov"
		set(hlavicka + sizeof(int), next_next);
		set(hlavicka + 2 * sizeof(int), next_prev);
		
		// Nastavenie hodnoty novej hlavicky
		set(hlavicka, get(hlavicka) + get(next_hlavicka) - 2 * sizeof(int));
		
		// Nastavenie hodnoty novej paticky
		set(next_paticka, get(hlavicka));
		
		// Uprava smernikov v prislusnych poliach
		if (next_next > 0) { set(next_next + 2 * sizeof(int), hlavicka); }
		if (next_prev > 0) { set(next_prev + sizeof(int), hlavicka); }
		
		set(hlavicka + sizeof(int), next_next);
		set(hlavicka + 2 * sizeof(int), next_prev);

		// Ak bol nasledujuci volny blok prvy volny
		if (get(sizeof(int)) == next_hlavicka) { set(sizeof(int), hlavicka); }

		if (debug) { vypis(); }
		return 0;
	}
	// Ak uvolnovanemu bloku predchadza dalsi volny blok
	else if ((prev_paticka > sizeof(int)) && (get(prev_paticka) < 0)) {
		unsigned int prev_dlzka = - get(prev_paticka);
		unsigned int prev_hlavicka = prev_paticka - prev_dlzka - sizeof(int);
		unsigned int new_dlzka = prev_dlzka + dlzka + 2 * sizeof(int);
		
		if (debug) {
			printf("Uvolnovanemu bloku predchadza dalsi volny\n");
			printf("prev_dlzka: %u\n", prev_dlzka);
			printf("prev_paticka: %u\n", prev_paticka);
			printf("new_dlzka: %d\n", new_dlzka);
		}
		
		set(prev_hlavicka, - new_dlzka);
		set(paticka, - new_dlzka);

		if (debug) { vypis(); }
		return 0;
	}
	
	set(hlavicka + sizeof(int), -1);
	set(hlavicka + 2 * sizeof(int), -1);
	
	// Ak je novy prazdny blok prvy v poradi
	if (hlavicka < get(sizeof(int))) {
		if (debug) {
			printf("Novy prazdny blok je prvy v poradi\n");
		}

		set(get(sizeof(int)) + 2 * sizeof(int), hlavicka);
		set(hlavicka + sizeof(int), get(sizeof(int)));
		set(hlavicka + 2 * sizeof(int), -1);
		set(sizeof(int), hlavicka);
	}
	// Ak nie je novy prazdny blok prvy v poradi
	else {
		int akt = get(sizeof(int));

		if (debug) {
			printf("Novy prazdny blok nie je prvy v poradi\n");
		}
		
		while (1) {
			int pom = get(akt + sizeof(int));

			if (debug) {
				printf("akt: %d\n", akt);
				printf("pom: %d\n", pom);
			}
			
			if (pom > 0) {
				if (pom < hlavicka) {
					akt = pom;
					continue;
				}
				else {
					set(akt + sizeof(int), hlavicka);
					set(hlavicka + sizeof(int), pom);
					set(hlavicka + 2 * sizeof(int), akt);
					set(pom + 2 * sizeof(int), hlavicka);
					break;
				}
			}
			else {
				set(akt + sizeof(int), hlavicka);
				set(hlavicka + sizeof(int), -1);
				set(hlavicka + 2 * sizeof(int), akt);
				break;
			}
		}
	}

	if (debug) { vypis(); }
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

// Sluzi na inicializaciu pamate, zbehne len raz
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
  char region[100];
  memory_init(region, 100);
  printf("Main dostal z memory_alloc: %d\n", memory_alloc(20) - pamat);
  vypis();
  getchar();
  printf("Main dostal z memory_alloc: %d\n", memory_alloc(20) - pamat);
  vypis();
  getchar();
  printf("Main dostal z memory_alloc: %d\n", memory_alloc(5) - pamat);
  vypis();
  getchar();
  printf("Memory free 40: %d\n", memory_free(region + 40));
  vypis();
  getchar();
  printf("Main dostal z memory_alloc: %d\n", memory_alloc(21) - pamat);
  vypis();
  getchar();
  printf("Memory free 12: %d\n", memory_free(region + 12));
  vypis();
  getchar();
  /*
  printf("Memory check 12: %d\n", memory_check(region + 12));
  printf("Memory free 12: %d\n", memory_free(region + 12));
  printf("Memory check 12: %d\n", memory_check(region + 12));
  
  char* pointer = (char*) memory_alloc(10);
  if (pointer)
    memset(pointer, 0, 10);
  if (pointer)
    memory_free(pointer);*/
  return 0;
}
