// zadanie2.c -- Zadanie 2 - Socialna siet
// Stanislav Jakubek, 28.10.2015 16:43:11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct uzivatel {
  char* meno;
  int pocetl, pocetp;
  struct uzivatel *l, *p;
} UZIVATEL;

typedef struct stranka {
  char* meno;
  UZIVATEL* uzivatelia;
} STRANKA;

STRANKA* pole[5107];
int velk_pola=5107, pomocna=0;

unsigned int hashFunc(char* s) {
  unsigned int sucet=0, i=0;
  
  while (s[i]) {
    sucet = sucet*31 + s[i];
    i++;
  }
  
  return sucet%velk_pola;
}

char *getuser(char *page, int k);

// Vlozi user do bin. stromu akt
void vlozUzivatela(UZIVATEL *akt, char *user) {
  while (1) {
    if (strcmp(akt->meno, user) == 0) {
      printf("Chyba: SU ROVNAKE\n");
    }
    if ((strcmp(user, akt->meno) < 0) && (akt->l == NULL)) {
      //akt->pocetl = akt->pocetl + 1;
      break;
    }
    else if ((strcmp(user, akt->meno) > 0) && (akt->p == NULL)) {
      //akt->pocetp = akt->pocetp + 1;
      break;
    }
    else if (strcmp(akt->meno, user) > 0) {
      akt->pocetl = akt->pocetl + 1;
      akt = akt->l;
    }
    else {
      akt->pocetp = akt->pocetp + 1;
      akt = akt->p;
    }
  }
  
  if ((strcmp(user, akt->meno) < 0) && (akt->l == NULL)) {
    akt->pocetl = akt->pocetl + 1;
    akt->l = (UZIVATEL *) malloc (sizeof(UZIVATEL));
    /*akt->l->meno = (char *) malloc ((strlen(user) + 1) * sizeof(char));
    strcpy(akt->l->meno, user);*/
    akt->l->meno = strdup(user);
    akt->l->l = NULL;
    akt->l->pocetl = 0;
    akt->l->p = NULL;
    akt->l->pocetp = 0;
  }
  else if ((strcmp(user, akt->meno) > 0) && (akt->p == NULL)) {
    akt->pocetp = akt->pocetp + 1;
    akt->p = (UZIVATEL *) malloc (sizeof(UZIVATEL));
    /*akt->p->meno = (char *) malloc ((strlen(user) + 1) * sizeof(char));
    strcpy(akt->p->meno, user);*/
    akt->p->meno = strdup(user);
    akt->p->l = NULL;
    akt->p->pocetl = 0;
    akt->p->p = NULL;
    akt->p->pocetp = 0;
  }
}

// Odoberie user z bin. stromu akt
void odoberUzivatela(UZIVATEL *akt, char *user) {
  UZIVATEL *pom, *pom2, *pred;
  
  // ak treba vymazat nie-koren
  while ((akt->l != NULL) || (akt->p != NULL)) {
    if ((akt->l != NULL) && (strcmp(akt->l->meno, user) == 0)) {
      akt->pocetl = akt->pocetl - 1;
      break;
    }
    else if ((akt->p != NULL) && (strcmp(akt->p->meno, user) == 0)) {
      akt->pocetp = akt->pocetp - 1;
      break;
    }
    
    if (strcmp(akt->meno, user) > 0) {
      akt->pocetl = akt->pocetl - 1;
      akt = akt->l;
    }
    else {
      akt->pocetp = akt->pocetp - 1;
      akt = akt->p;
    }
  }
  
  if ((akt->l != NULL) && (strcmp(akt->l->meno, user) == 0)) {
    //odoberie laveho potomka
    pom = akt->l;
    if (pom->l == NULL) {
      //ak ten nema laveho
      akt->l = pom->p;
      //free(pom);
      pom = NULL;
    }
    else if (pom->p == NULL) {
      //ak ten nema praveho
      akt->l = pom->l;
      //free(pom);
      pom = NULL;
    }
    else {
      //ak ten ma oboch
      pred = pom;
      pom2 = pom->l;
      if (pom2->p == NULL) {
        pom->pocetl = pom2->pocetl;
        strcpy(pom->meno, pom2->meno);
        pom->l = pom2->l;
        //free(pom2);
        pom2 = NULL;
      }
      else {
        while (pom2->p != NULL) {
          if (pred->l == pom2) { pred->pocetl = pred->pocetl - 1; }
          else { pred->pocetp = pred->pocetp - 1; }
          pred = pom2;
          pom2 = pom2->p;
        }
        strcpy(pom->meno, pom2->meno);
        pred->p = pom2->l;
        //free(pom2);
        pom2 = NULL;
      }
    }
  }
  else if ((akt->p != NULL) && (strcmp(akt->p->meno, user) == 0)) {
    //odoberie praveho potomka
    pom = akt->p;
    if (pom->l == NULL) {
      //ak ten nema laveho
      akt->p = pom->p;
      //free(pom);
      pom = NULL;
    }
    else if (pom->p == NULL) {
      //ak ten nema praveho
      akt->p = pom->l;
      //free(pom);
      pom = NULL;
    }
    else {
      //ak ten ma oboch
      pred = pom;
      pom2 = pom->l;
      if (pom2->p == NULL) {
        pom->pocetl = pom2->pocetl;
        strcpy(pom->meno, pom2->meno);
        pom->l = pom2->l;
        //free(pom2);
        pom2 = NULL;
      }
      else {
        while (pom2->p != NULL) {
          if (pred->l == pom2) { pred->pocetl = pred->pocetl - 1; }
          else { pred->pocetp = pred->pocetp - 1; }
          pred = pom2;
          pom2 = pom2->p;
        }
        strcpy(pom->meno, pom2->meno);
        pred->p = pom2->l;
        //free(pom2);
        pom2 = NULL;
      }
    }
  }
}

char* vratUzivatela(UZIVATEL *akt, int k) {
  //printf("VSTUP %d ", k);
  //printf("VSTUP K: %d P: %d ", k, (akt->pocetl + akt->pocetp + 1));
  int pom = k;
  
  if (k > (akt->pocetl + akt->pocetp + 1)) { /*if(pomocna==1) printf("NULL\n");pomocna=0;*/return NULL; }
  else {
    while (pom != (akt->pocetl + 1)) {
      //printf("TEST %d L: %d P: %d ", pom, akt->pocetl, akt->pocetp);
      if (pom <= akt->pocetl) {
        akt = akt->l;
      }
      else {
        pom = pom - akt->pocetl - 1;
        akt = akt->p;
      }
    }
    //if(pomocna==1) printf("%s\n", akt->meno);
    //pomocna=0;
    return strdup(akt->meno);
  }
  return NULL;
}

void init() {
  int i;
  for (i = 0; i < velk_pola; i++)
    pole[i] = NULL;
}

void like(char *page, char *user) {
  int j, hf = hashFunc(page);
  
  /*if (strcmp(page, "CVb][Is=1<>:S[IGN2jZ_?4]=<tiN9/Pp;_@GYt`R") == 0)
    printf("Like\n%s %s %d\n", page, user, hf);*/
  
  //printf("L\n");
  //printf("Debug: LIKE %s %s\n", page, user);
  
  if (pole[hf] == NULL) {
    //printf("Debug: Prve vytvorenie stranky %s\n", page);
    pole[hf] = (STRANKA *) malloc (sizeof(STRANKA));
    pole[hf]->meno = (char *) malloc ((strlen(page) + 1) * sizeof(char));
    strcpy(pole[hf]->meno, page);
    pole[hf]->uzivatelia = (UZIVATEL *) malloc (sizeof(UZIVATEL));
    /*pole[hf]->uzivatelia->meno = (char *) malloc ((strlen(user) + 1) * sizeof(char));
    strcpy(pole[hf]->uzivatelia->meno, user);*/
    pole[hf]->uzivatelia->meno = strdup(user);
    pole[hf]->uzivatelia->pocetl = 0;
    pole[hf]->uzivatelia->pocetp = 0;
    pole[hf]->uzivatelia->l = NULL;
    pole[hf]->uzivatelia->p = NULL;
  }
  else if (strcmp(pole[hf]->meno, page) == 0) {
    //printf("Debug: Stranka uz existuje %s\n", page);
    vlozUzivatela(pole[hf]->uzivatelia, user);
  }
  else {
    j = hf;
    while (pole[j] != NULL) {
      if (strcmp(pole[j]->meno, page) == 0) {
        break;
      }
      
      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }
    if (pole[j] == NULL) {
      //printf("Debug: Prve vytvorenie stranky po kolizii\n");
      pole[j] = (STRANKA *) malloc (sizeof(STRANKA));
      pole[j]->meno = (char *) malloc ((strlen(user) + 1) * sizeof(char));
      strcpy(pole[j]->meno, page);
      pole[j]->uzivatelia = (UZIVATEL *) malloc (sizeof(UZIVATEL));
      /*pole[j]->uzivatelia->meno = (char *) malloc ((strlen(user) + 1) * sizeof(char));
      strcpy(pole[j]->uzivatelia->meno, user);*/
      pole[j]->uzivatelia->meno = strdup(user);
      pole[j]->uzivatelia->pocetl = 0;
      pole[j]->uzivatelia->pocetp = 0;
      pole[j]->uzivatelia->l = NULL;
      pole[j]->uzivatelia->p = NULL;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      //printf("Debug: Stranka uz existuje po kolizii\n");
      vlozUzivatela(pole[j]->uzivatelia, user);
    }
  }
}
      
void unlike(char *page, char *user) {
  int j, hf = hashFunc(page);
  
  /*if (strcmp(page, "Vb][Is=1<>:S[IGN2jZ_?4]=<tiN9/Pp;_@GYt`R") == 0)
    printf("Unlike %d\n%s %s\n",hf,page,user);*/
  
  
  UZIVATEL *pred, *pom;
  unsigned int poc = 0;	//kontrola ci zbehol while
  //printf("Debug: UNLIKE %s %s\n", page, user);
  //printf("Un\n");
  hf = hashFunc(page);
  
  if (pole[hf] == NULL) { /*printf("Chyba: NULL Unlike\n");*/ return; }
  
  else if (strcmp(pole[hf]->meno, page) == 0) {
    if ((strcmp(pole[hf]->uzivatelia->meno, user) == 0) && (pole[hf]->uzivatelia->l == NULL) && (pole[hf]->uzivatelia->p == NULL)){
      //printf("Vymazanie posledneho (jedineho) like-u\n");
      pole[hf] = NULL;
      return;
    }
    else if ((strcmp(pole[hf]->uzivatelia->meno, user) == 0) && (pole[hf]->uzivatelia->l == NULL)){
      //printf("Odobera koren, vlavo nic neni\n");
      *pole[hf]->uzivatelia = *pole[hf]->uzivatelia->p;
      return;
    }
    else if ((strcmp(pole[hf]->uzivatelia->meno, user) == 0) && (pole[hf]->uzivatelia->p == NULL)){
      //printf("Odobera koren, vpravo nic neni\n");
      *pole[hf]->uzivatelia = *pole[hf]->uzivatelia->l;
      return;
    }
    else if (strcmp(pole[hf]->uzivatelia->meno, user) == 0) {
      //printf("Odobera koren, ma obidvoch potomkov\n");
      
      pole[hf]->uzivatelia->pocetl = (pole[hf]->uzivatelia->pocetl - 1);
      pred = pole[hf]->uzivatelia;
      pom = pole[hf]->uzivatelia->l;
      while (pom->p != NULL) {
        pom->pocetp = (pom->pocetp - 1);
        pred = pom;
        pom = pom->p;
        poc++;
      }
      if (poc == 0) {
        strcpy(pole[hf]->uzivatelia->meno, pom->meno);
        if (pom->l == NULL) {
          pole[hf]->uzivatelia->l = NULL;
          pole[hf]->uzivatelia->pocetl = 0;
        }
        else *pole[hf]->uzivatelia->l = *pom->l;
      }
      else {
        strcpy(pole[hf]->uzivatelia->meno, pom->meno);
        if (pom->l == NULL) {
          pred->p = NULL;
          pred->pocetp = 0;
        }
        else pred->p = pom->l;
      }
      //pom = NULL;
      //free(pom);
      return;
    }
    odoberUzivatela(pole[hf]->uzivatelia, user);
  }
  else {
    j = hf;
    while (pole[j] != NULL) {
      if (strcmp(pole[j]->meno, page) == 0) {
        break;
      }
      
      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }
    if (pole[j] == NULL) {
      //printf("Debug: Stranka neexistuje po kolizii\n");
      return;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      //printf("Debug: Stranka uz existuje po kolizii\n");
      if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->l == NULL) && (pole[j]->uzivatelia->p == NULL)) {
        //printf("Vymazanie posledneho (jedineho) like-u po kolizii\n");
        pole[j] = NULL;
        return;
      }
      else if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->l == NULL)){
        //printf("Odobera koren, vlavo nic neni po kolizii\n");
        *pole[j]->uzivatelia = *pole[j]->uzivatelia->p;
        return;
      }
      else if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->p == NULL)){
        //printf("Odobera koren, vpravo nic neni po kolizii\n");
        *pole[j]->uzivatelia = *pole[j]->uzivatelia->l;
        return;
      }
      else if (strcmp(pole[j]->uzivatelia->meno, user) == 0) {
        //printf("Odobera koren, ma obidvoch potomkov po kolizii\n");
        
        pole[j]->uzivatelia->pocetl = (pole[j]->uzivatelia->pocetl - 1);
        pred = pole[j]->uzivatelia;
        pom = pole[j]->uzivatelia->l;
        while (pom->p != NULL) {
          pom->pocetp = (pom->pocetp - 1);
          pred = pom;
          pom = pom->p;
          poc++;
        }
        if (poc == 0) {
          strcpy(pole[j]->uzivatelia->meno, pom->meno);
          if (pom->l == NULL) {
            pole[j]->uzivatelia->l = NULL;
            pole[j]->uzivatelia->pocetl = 0;
          }
          else *pole[j]->uzivatelia->l = *pom->l;
        }
        else {
          strcpy(pole[j]->uzivatelia->meno, pom->meno);
          if (pom->l == NULL) {
            pred->p = NULL;
            pred->pocetp = 0;
          }
          else pred->p = pom->l;
        }
        //pom = NULL;
        //free(pom);
        return;
      }
      //printf("Vymazanie ne-korena po kolizii\n");
      odoberUzivatela(pole[j]->uzivatelia, user);
    }
  }
}

char *getuser(char *page, int k) {
  int j, hf = hashFunc(page);
  
  /*if (strcmp(page, "Vb][Is=1<>:S[IGN2jZ_?4]=<tiN9/Pp;_@GYt`R") == 0) {
    printf("Getuser %d %d\n",k,hf);
    pomocna = 1;
  }*/
  
  //printf("G\n");
  
  if (pole[hf] == NULL) { /*printf("Getuser NULL %s\n", page);*/return NULL; }
  
  else if (strcmp(pole[hf]->meno, page) == 0) {
    //printf("Debug: Stranka uz existuje\n");
    return vratUzivatela(pole[hf]->uzivatelia, k);
  }
  else {
    j = hf;
    while (pole[j] != NULL) {
      if (strcmp(pole[j]->meno, page) == 0) {
        break;
      }
      
      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }
    if (pole[j] == NULL) {
      //printf("Debug: Stranka neexistuje po kolizii\n");
      return NULL;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      //printf("Debug: Stranka uz existuje po kolizii\n");
    return vratUzivatela(pole[j]->uzivatelia, k);
    }
  }
  return NULL;
}
            
// Tlacidlo 'Spustit' vyskusa vasu socialnu siet na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.

