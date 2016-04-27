// zadanie3.c -- Zadanie 3 - Popolvar
// Stanislav Jakubek, 30.11.2015 09:05:03

#include <stdio.h>
#include <stdlib.h>

typedef struct cesta {
  int a,b;
  int dlzka;
  struct cesta *pred;
} CESTA;

typedef struct fronta {
  int dlzka;
  struct fronta *dalsi;
  CESTA *cesta;
} FRONTA;

CESTA *drak, *dp1, *dp2, *dp3, *p12, *p13, *p21, *p23, *p31, *p32;

void init() {
  drak = NULL;
  dp1 = NULL;
  dp2 = NULL;
  dp3 = NULL;
  p12 = NULL;
  p13 = NULL;
  p21 = NULL;
  p23 = NULL;
  p31 = NULL;
  p32 = NULL;
}

void addFronta(FRONTA **fronta, char **mapa, int a, int b) {
  FRONTA *akt = *fronta, *novy = NULL;
  int dlzkaCesty;
  
  akt = *fronta;
  
  if (mapa[a][b] == 'n') { return; }
  else if (mapa[a][b] == 'h') { dlzkaCesty = 2; }
  else { dlzkaCesty = 1; }
  
  novy = NULL;
  novy = (FRONTA *) malloc (sizeof(FRONTA));
  novy->cesta = (CESTA *) malloc (sizeof(CESTA));
  novy->cesta->a = a;
  novy->cesta->b = b;
  novy->cesta->pred = (*fronta)->cesta;
  novy->dlzka = (*fronta)->dlzka + dlzkaCesty;
  
  while (akt->dalsi != NULL) {
    if ((akt->dalsi->cesta->a == a) && (akt->dalsi->cesta->b == b)) {
      // ak vo fronte uz je do tohto bodu ina cesta
      if (akt->dalsi->dlzka > novy->dlzka) {
        // ak je tato cesta dlhsia
        akt->dalsi = akt->dalsi->dalsi;
        break;
      }
      else if (akt->dalsi->dlzka <= novy->dlzka) {
        // ak je tato cesta kratsia
        return;
      }
    }
    
    akt = akt->dalsi;
  }
  
  akt = (*fronta);
  
  while (akt->dalsi != NULL) {
    if (novy->dlzka < akt->dalsi->dlzka) { break; }
    akt = akt->dalsi;
  }
  
  novy->dalsi = akt->dalsi;
  akt->dalsi = novy;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
  int *returnPole = NULL, pomPole[6] = {2147483647}, minValue=2147483647, minPos;
  int a, b, g, h, i, j, k, l;
  FRONTA *fronta = NULL, *temp = NULL;
  char pomMapa[n][m];	// Sluzi na zapamatanie navstivenia miest na mape
  CESTA *pom = NULL, *pom1 = NULL, *pom2 = NULL, *pom3 = NULL;
  
  init();
  
  /*for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++)
      printf("%c ", mapa[i][j]);
    printf("\n");
  }*/
  
  // 5-krat Dijkstrov algoritmus, kazdy krat robi nieco ine
  for (i = 0; i < 5; i++) {
    //printf("I: %d\n", i);
    for (j = 0; j < n; j++)
      for (k = 0; k < m; k++)
        pomMapa[j][k] = '0';
    
    switch (i) {
      case 0 : a = 0;		b = 0;	    break;	// Hladanie draka od [0][0]
      case 1 : a = drak->a;	b = drak->b;break;	// Hladanie princezien od draka
      case 2 : a = dp1->a;	b = dp1->b; break;	// Hladanie P2 a P3 od P1
      case 3 : a = dp2->a;	b = dp2->b; break;	// Hladanie P1 a P3 od P2
      case 4 : a = dp3->a;	b = dp3->b; break;	// Hladanie P1 a P2 od P3
    }
    
    fronta = NULL;
    fronta = malloc (sizeof(FRONTA*));
    fronta->cesta = (CESTA *) malloc (sizeof(CESTA));
    fronta->cesta->a = a;
    fronta->cesta->b = b;
    fronta->cesta->pred = NULL;
    fronta->dlzka = 0;
    fronta->dalsi = NULL;
    
    while (fronta != NULL) {
      a = fronta->cesta->a;
      b = fronta->cesta->b;
      pomMapa[a][b] = '1';
      
      if ((i == 0) && (mapa[a][b] == 'd')) {
        //printf("TESTDRAK\n");
        drak = fronta->cesta;
        drak->dlzka = fronta->dlzka;
        break;
      }
      else if ((i == 1) && (mapa[a][b] == 'p')) {
        //printf("TESTDDPP\n");
        if (dp1 == NULL) { dp1 = fronta->cesta; dp1->dlzka = fronta->dlzka; }
        else if (dp2 == NULL) { dp2 = fronta->cesta; dp2->dlzka = fronta->dlzka; }
        else if (dp3 == NULL) { dp3 = fronta->cesta; dp3->dlzka = fronta->dlzka; free(fronta); break; }
      }
      else if ((i == 2) && (mapa[a][b] == 'p') && !((a == dp1->a) && (b == dp1->b))) {
        //printf("TEST1111\n");
        /*if ((p13 == NULL) && (a == dp2->a) && (b == dp2->b)) { p12 = fronta->cesta; p12->dlzka = fronta->dlzka; }
        else if ((p13 != NULL) && (a == dp2->a) && (b == dp2->b)) { p12 = fronta->cesta; p12->dlzka = fronta->dlzka; free(fronta); break; }
        else if ((p12 == NULL) && (a == dp3->a) && (b == dp3->b)) { p13 = fronta->cesta; p13->dlzka = fronta->dlzka; }
        else if ((p12 != NULL) && (a == dp3->a) && (b == dp3->b)) { p13 = fronta->cesta; p13->dlzka = fronta->dlzka; free(fronta); break; }*/
        if (p12 == NULL) { p12 = fronta->cesta; p12->dlzka = fronta->dlzka; }
        else if (p13 == NULL) { p13 = fronta->cesta; p13->dlzka = fronta->dlzka; free(fronta); fronta = NULL; break; }
      }
      else if ((i == 3) && (mapa[a][b] == 'p') && !((a == dp2->a) && (b == dp2->b))) {
        //printf("TEST2222\n");
        if ((p23 == NULL) && (a == dp1->a) && (b == dp1->b)) { p21 = fronta->cesta; p21->dlzka = fronta->dlzka; }
        else if ((p23 != NULL) && (a == dp1->a) && (b == dp1->b)) { p21 = fronta->cesta; p21->dlzka = fronta->dlzka; free(fronta); break; }
        else if ((p21 == NULL) && (a == dp3->a) && (b == dp3->b)) { p23 = fronta->cesta; p23->dlzka = fronta->dlzka; }
        else if ((p21 != NULL) && (a == dp3->a) && (b == dp3->b)) { p23 = fronta->cesta; p23->dlzka = fronta->dlzka; free(fronta); break; }
      }
      else if ((i == 4) && (mapa[a][b] == 'p') && !((a == dp3->a) && (b == dp3->b))) {
        //printf("TEST3333\n");
        if ((p32 == NULL) && (a == dp1->a) && (b == dp1->b)) { p31 = fronta->cesta; p31->dlzka = fronta->dlzka; }
        else if ((p32 != NULL) && (a == dp1->a) && (b == dp1->b)) { p31 = fronta->cesta; p31->dlzka = fronta->dlzka; free(fronta); break; }
        else if ((p31 == NULL) && (a == dp2->a) && (b == dp2->b)) { p32 = fronta->cesta; p32->dlzka = fronta->dlzka; }
        else if ((p31 != NULL) && (a == dp2->a) && (b == dp2->b)) { p32 = fronta->cesta; p32->dlzka = fronta->dlzka; free(fronta); break; }
      }
      
      if (a == 0) {
        if (pomMapa[a+1][b] == '0') addFronta(&fronta, mapa, a+1, b);
        
        if (b == 0) {
          // lavy horny roh
          if (pomMapa[a][b+1] == '0') addFronta(&fronta, mapa, a, b+1);
        }
        else if (b == (m-1)) {
          // pravy horny roh
          if (pomMapa[a][b-1] == '0') addFronta(&fronta, mapa, a, b-1);
        }
        else {
          // horna hrana
          if (pomMapa[a][b+1] == '0') addFronta(&fronta, mapa, a, b+1);
          if (pomMapa[a][b-1] == '0') addFronta(&fronta, mapa, a, b-1);
        }
      }
      else if (a == (n-1)) {
        if (pomMapa[a-1][b] == '0') addFronta(&fronta, mapa, a-1, b);
        
        if (b == 0) {
          // lavy dolny roh
          if (pomMapa[a][b+1] == '0') addFronta(&fronta, mapa, a, b+1);
        }
        else if (b == (m-1)) {
          // pravy dolny roh
          if (pomMapa[a][b-1] == '0') addFronta(&fronta, mapa, a, b-1);
        }
        else {
          // dolna hrana
          if (pomMapa[a][b+1] == '0') addFronta(&fronta, mapa, a, b+1);
          if (pomMapa[a][b-1] == '0') addFronta(&fronta, mapa, a, b-1);
        }
      }
      else if (b == 0) {
        // lava hrana
        if (pomMapa[a][b+1] == '0') addFronta(&fronta, mapa, a, b+1);
        if (pomMapa[a+1][b] == '0') addFronta(&fronta, mapa, a+1, b);
        if (pomMapa[a-1][b] == '0') addFronta(&fronta, mapa, a-1, b);
      }
      else if (b == (m-1)) {
        // prava hrana
        if (pomMapa[a][b-1] == '0') addFronta(&fronta, mapa, a, b-1);
        if (pomMapa[a+1][b] == '0') addFronta(&fronta, mapa, a+1, b);
        if (pomMapa[a-1][b] == '0') addFronta(&fronta, mapa, a-1, b);
      }
      else {
        if (pomMapa[a][b+1] == '0') addFronta(&fronta, mapa, a, b+1);
        if (pomMapa[a][b-1] == '0') addFronta(&fronta, mapa, a, b-1);
        if (pomMapa[a+1][b] == '0') addFronta(&fronta, mapa, a+1, b);
        if (pomMapa[a-1][b] == '0') addFronta(&fronta, mapa, a-1, b);
      }
      
      // Odstranenie prveho prvku fronty a posun na dalsi
      temp = fronta;
      if (fronta != NULL) {
        fronta = fronta->dalsi;
        free(temp);
      }
    }
  }
  
  //printf("Dijkstrove algoritmy uz prebehli\n");
  
  /*printf("POMOCNY VYPIS\n");
  pom = dp1;
  while (pom != NULL) {
    printf("%d %d\n ", pom->a, pom->b);
    pom = pom->pred;
  }
  printf("POMOCNY VYPIS\n");*/
  
  if (drak == NULL) {
    (*dlzka_cesty) = 0;
    return NULL;
  }
  
  // Kombinacie vzdialenosti
  pomPole[0] = (drak->dlzka + dp1->dlzka + p12->dlzka + p23->dlzka);
  pomPole[1] = (drak->dlzka + dp1->dlzka + p13->dlzka + p32->dlzka);
  pomPole[2] = (drak->dlzka + dp2->dlzka + p21->dlzka + p13->dlzka);
  pomPole[3] = (drak->dlzka + dp2->dlzka + p23->dlzka + p31->dlzka);
  pomPole[4] = (drak->dlzka + dp3->dlzka + p31->dlzka + p12->dlzka);
  pomPole[5] = (drak->dlzka + dp3->dlzka + p32->dlzka + p21->dlzka);
  
  // Najdenie najkratsej cesty
  for (i = 0; i < 6; i++) {
    if (pomPole[i] < minValue) {
      minValue = pomPole[i];
      minPos = i;
    }
  }
  
  switch (minPos) {
    case 0 :
    	//printf("TEST0 %d\n", minValue);
    	pom1 = dp1;
        pom2 = p12;
        pom3 = p23;
    	break;
    case 1 :
    	//printf("TEST1 %d\n", minValue);
        pom1 = dp1;
        pom2 = p13;
        pom3 = p32;
    	break;
    case 2 :
    	//printf("TEST2 %d\n", minValue);
        pom1 = dp2;
        pom2 = p21;
        pom3 = p13;
    	break;
    case 3 :
    	//printf("TEST3 %d\n", minValue);
        pom1 = dp2;
        pom2 = p23;
        pom3 = p31;
    	break;
    case 4 :
    	//printf("TEST4 %d\n", minValue);
        pom1 = dp3;
        pom2 = p31;
        pom3 = p12;
    	break;
    case 5 :
    	//printf("TEST5 %d\n", minValue);
        pom1 = dp3;
        pom2 = p32;
        pom3 = p21;
    	break;
  }
  
  // vypocitanie dlzky cesty
  for (i = 0; i < 4; i++) {
    switch (i) {
      case 0 :
      	pom = drak;
      	break;
      case 1 :
      	pom = pom1;
      	(*dlzka_cesty)--;
      	break;
      case 2 :
      	pom = pom2;
      	(*dlzka_cesty)--;
      	break;
      case 3 :
      	pom = pom3;
      	(*dlzka_cesty)--;
      	break;
    }
    
    while (pom != NULL) {
      (*dlzka_cesty)++;
      pom = pom->pred;
    }
  }
  
  //printf("DC: %d\n", *dlzka_cesty);
  returnPole = malloc(2 * (*dlzka_cesty) * sizeof(int));
  
  j = 0;
  for (i = 0; i < 4; i++) {
    switch (i) {
      case 0 :
      	pom = drak;
      	break;
      case 1 :
      	pom = pom1;
      	break;
      case 2 :
      	pom = pom2;
      	break;
      case 3 :
      	pom = pom3;
      	break;
    }
    g = j*2;
    // Pridavanie prvkov do returnPole
    switch (i) {
      case 0 :
      	while (pom != NULL) {
          returnPole[j*2] = pom->b;
          returnPole[j*2+1] = pom->a;
          j++;
          pom = pom->pred;
        }
      	break;
      case 1 :
      case 2 :
      case 3 :
      	while (pom->pred != NULL) {
          returnPole[j*2] = pom->b;
          returnPole[j*2+1] = pom->a;
          j++;
          pom = pom->pred;
        }
      	break;
    }
    
    // "Obratenie" pridanych prvkov
    h = j*2-1;
    while (g != h+1) {
      l = returnPole[g];
      returnPole[g] = returnPole[h];
      returnPole[h] = l;
      g++;
      h--;
    }
  }
  
  //printf("J: %d\n", j);
  /*for (i = 0; i < 2*(*dlzka_cesty); i++) {
    printf("%d: %d\n", i, returnPole[i]);
  }*/
  
  //printf("Cely program zbehol\n");
  return returnPole;
}
            
// Tlacidlo 'Spustit' vyskusa vasu socialnu siet na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.

