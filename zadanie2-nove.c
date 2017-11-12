// zadanie2.c -- Stanislav Jakúbek, 4.11.2017 16:50
// zadanie2.c -- Zadanie 2 - Socialna siet
// Stanislav Jakubek, 28.10.2015 16:43:11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug 0
#define velk_pola 2003

typedef struct uzivatel {
  char* meno;
  int pocetl, pocetp;
  struct uzivatel *l, *p;
} UZIVATEL;

typedef struct stranka {
  char* meno;
  UZIVATEL* uzivatelia;
} STRANKA;

STRANKA* pole[velk_pola];

unsigned int hashFunc(char* s) {
  unsigned int sucet=0, i=0;
  
  while (s[i]) {
    sucet = sucet*31 + s[i];
    i++;
  }
  
  if (debug) {
    printf("hashFunc: %d\n", sucet%velk_pola);
  }

  return sucet%velk_pola;
}

char *getuser(char *page, int k);

// Vlozi user do bin. stromu akt
void vlozUzivatela(UZIVATEL *akt, char *user) {
  if (debug) {
    printf("vlozUzivatela\n");
    printf("akt->meno: %s\n", akt->meno);
    printf("akt->pocetl: %d\n", akt->pocetl);
    printf("akt->pocetp: %d\n", akt->pocetp);
  }

  while (1) {
    if ((strcmp(user, akt->meno) < 0) && (akt->l == NULL)) {
      if (debug) {
        printf("Vlavo je volno\n");
      }

      break;
    }
    else if ((strcmp(user, akt->meno) > 0) && (akt->p == NULL)) {
      if (debug) {
        printf("Vpravo je volno\n");
      }

      break;
    }
    else if (strcmp(akt->meno, user) > 0) {
      if (debug) {
        printf("Meno je mensie nez %s\n", akt->meno);
        printf("akt->pocetl + 1: %d\n", akt->pocetl + 1);
      }
      akt->pocetl = akt->pocetl + 1;
      akt = akt->l;
    }
    else {
      if (debug) {
        printf("Meno je vacsie nez %s\n", akt->meno);
        printf("akt->pocetp + 1: %d\n", akt->pocetp + 1);
      }
      akt->pocetp = akt->pocetp + 1;
      akt = akt->p;
    }
  }
  
  if ((strcmp(user, akt->meno) < 0) && (akt->l == NULL)) {
    akt->pocetl = akt->pocetl + 1;
    akt->l = (UZIVATEL *) malloc (sizeof(UZIVATEL));
    akt->l->meno = strdup(user);
    akt->l->l = NULL;
    akt->l->pocetl = 0;
    akt->l->p = NULL;
    akt->l->pocetp = 0;
  }
  else if ((strcmp(user, akt->meno) > 0) && (akt->p == NULL)) {
    akt->pocetp = akt->pocetp + 1;
    akt->p = (UZIVATEL *) malloc (sizeof(UZIVATEL));
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

  if (debug) {
    printf("odoberUzivatela\n");
    printf("user: %s\n", user);
  }
  
  while ((akt->l != NULL) || (akt->p != NULL)) {
    if ((akt->l != NULL) && (strcmp(akt->l->meno, user) == 0)) {
      if (debug) {
        printf("odoberany je vlavo\n");
      }

      akt->pocetl = akt->pocetl - 1;
      break;
    }
    else if ((akt->p != NULL) && (strcmp(akt->p->meno, user) == 0)) {
      if (debug) {
        printf("odoberany je vpravo");
      }

      akt->pocetp = akt->pocetp - 1;
      break;
    }
    
    if (strcmp(akt->meno, user) > 0) {
      if (debug) {
        printf("odoberany je v lavom podstrome\n");
      }

      akt->pocetl = akt->pocetl - 1;
      akt = akt->l;
    }
    else {
      if (debug) {
        printf("odoberany je v pravom podstrome\n");
      }

      akt->pocetp = akt->pocetp - 1;
      akt = akt->p;
    }
  }
  
  if ((akt->l != NULL) && (strcmp(akt->l->meno, user) == 0)) {
    if (debug) {
      printf("odoberame laveho potomka\n");
    }

    pom = akt->l;

    if (pom->l == NULL) {
      if (debug) {
        printf("ten nema laveho potomka\n");
      }

      akt->l = pom->p;
      pom = NULL;
    }
    else if (pom->p == NULL) {
      if (debug) {
        printf("ten nema praveho potomka\n");
      }
      
      akt->l = pom->l;
      pom = NULL;
    }
    else {
      if (debug) {
        printf("ten ma oboch potomkov\n");
      }
      
      pred = pom;
      pom2 = pom->l;

      if (pom2->p == NULL) {
        pom->pocetl = pom2->pocetl;
        strcpy(pom->meno, pom2->meno);
        pom->l = pom2->l;
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
        pom2 = NULL;
      }
    }
  }
  else if ((akt->p != NULL) && (strcmp(akt->p->meno, user) == 0)) {
    if (debug) {
      printf("odoberame praveho potomka\n");
    }
    
    pom = akt->p;

    if (pom->l == NULL) {
      if (debug) {
        printf("ten nema laveho potomka\n");
      }

      akt->p = pom->p;
      pom = NULL;
    }
    else if (pom->p == NULL) {
      if (debug) {
        printf("ten nema praveho potomka\n");
      }
      
      akt->p = pom->l;
      pom = NULL;
    }
    else {
      if (debug) {
        printf("ten ma oboch potomkov\n");
      }
      
      pred = pom;
      pom2 = pom->l;

      if (pom2->p == NULL) {
        pom->pocetl = pom2->pocetl;
        strcpy(pom->meno, pom2->meno);
        pom->l = pom2->l;
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
        pom2 = NULL;
      }
    }
  }
}

char* vratUzivatela(UZIVATEL *akt, int k) {
  int pom = k;
  
  if (debug) {
    printf("vratUzivatela\n");
  }

  if (k > (akt->pocetl + akt->pocetp + 1)) {
    if (debug) {
      printf("Tolko pouzivatelov nie je!\n");
    }

    return NULL;
  }
  else {
    while (pom != (akt->pocetl + 1)) {
      if (pom <= akt->pocetl) {
        akt = akt->l;
      }
      else {
        pom = pom - akt->pocetl - 1;
        akt = akt->p;
      }
    }
    
    if (debug) {
      printf("Meno: %s\n", akt->meno);
    }

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
  if (debug) {
    printf("LIKE\n");
    printf("page: %s\n", page);
    printf("user: %s\n", user);
  }

  int j, hf = hashFunc(page);

  // Ak stranka neexistuje alebo nastane
  // kolizia s neexistujucou (NULL) strankou
  if (pole[hf] == NULL) {
    if (hf == (velk_pola - 1)) { j = 0; }
    else { j = hf + 1; }
    
    while (j != hf) {
      if ((pole[j] != NULL) && (strcmp(pole[j]->meno, page) == 0)) {
        break;
      }

      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }

    if (pole[j] == NULL) {
      if (debug) {
        printf("Prve vytvorenie stranky\n");
      }

      pole[hf] = (STRANKA *) malloc (sizeof(STRANKA));
      pole[hf]->meno = (char *) malloc ((strlen(page) + 1) * sizeof(char));
      strcpy(pole[hf]->meno, page);
      pole[hf]->uzivatelia = (UZIVATEL *) malloc (sizeof(UZIVATEL));
      pole[hf]->uzivatelia->meno = strdup(user);
      pole[hf]->uzivatelia->pocetl = 0;
      pole[hf]->uzivatelia->pocetp = 0;
      pole[hf]->uzivatelia->l = NULL;
      pole[hf]->uzivatelia->p = NULL;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      if (debug) {
        printf("Stranka uz existuje po kolizii\n");
      }

      vlozUzivatela(pole[j]->uzivatelia, user);
    }
  }
  else if (strcmp(pole[hf]->meno, page) == 0) {
    if (debug) {
      printf("Stranka uz existuje\n");
    }

    vlozUzivatela(pole[hf]->uzivatelia, user);
  }
  else {
    j = hf + 1;

    // Zisti ci stranka uz existuje
    while (j != hf) {
      if ((pole[j] != NULL) && (strcmp(pole[j]->meno, page) == 0)) {
        break;
      }
      
      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }

    // Ak neexistuje tak najde prvy volny blok
    if (strcmp(pole[j]->meno, page) != 0) {
      if (debug) {
        printf("Stranka neexistuje po kolizii");
      }

      while (pole[j] != NULL) {
        if (j == (velk_pola - 1)) { j = 0; }
        else { j++; }
      }
    }

    if (debug) {
      printf("kolizia, j: %d\n", j);
    }

    if (pole[j] == NULL) {
      if (debug) {
        printf("Prve vytvorenie stranky po kolizii\n");
      }

      pole[j] = (STRANKA *) malloc (sizeof(STRANKA));
      pole[j]->meno = (char *) malloc ((strlen(user) + 1) * sizeof(char));
      strcpy(pole[j]->meno, page);
      pole[j]->uzivatelia = (UZIVATEL *) malloc (sizeof(UZIVATEL));
      pole[j]->uzivatelia->meno = strdup(user);
      pole[j]->uzivatelia->pocetl = 0;
      pole[j]->uzivatelia->pocetp = 0;
      pole[j]->uzivatelia->l = NULL;
      pole[j]->uzivatelia->p = NULL;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      if (debug) {
        printf("Stranka uz existuje po kolizii\n");
      }

      vlozUzivatela(pole[j]->uzivatelia, user);
    }
  }
}
      
void unlike(char *page, char *user) {
  if (debug) {
    printf("UNLIKE\n");
    printf("page: %s\n", page);
    printf("user: %s\n", user);
  }

  int j, hf = hashFunc(page);
  
  UZIVATEL *pred, *pom;
  unsigned int poc = 0;	//kontrola ci zbehol while
  
  // Ak stranka neexistuje alebo nastane
  // kolizia s neexistujucou (NULL) strankou
  if (pole[hf] == NULL) {
    if (hf == (velk_pola - 1)) { j = 0; }
    else { j = hf + 1; }
    
    while (j != hf) {
      if ((pole[j] != NULL) && (strcmp(pole[j]->meno, page) == 0)) {
        break;
      }

      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }

    if (pole[j] == NULL) {
      if (debug) {
        printf("Stranka neexistuje\n");
      }

      return;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      if (debug) {
        printf("kolizia, j: %d\n", j);
      }
      
      if (pole[j] == NULL) {
        if (debug) {
          printf("Stranka neexistuje po kolizii\n");
        }
        
        return;
      }
      else if (strcmp(pole[j]->meno, page) == 0) {
        if (debug) {
          printf("Stranka uz existuje po kolizii\n");
        }
        
        if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->l == NULL) && (pole[j]->uzivatelia->p == NULL)) {
          if (debug) {
            printf("Vymazanie posledneho (jedineho) like-u po kolizii\n");
          }
          
          pole[j] = NULL;
          return;
        }
        else if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->l == NULL)) {
          if (debug) {
            printf("Odobera koren, vlavo nic neni po kolizii\n");
          }
          
          *pole[j]->uzivatelia = *pole[j]->uzivatelia->p;
          return;
        }
        else if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->p == NULL)){
          if (debug) {
            printf("Odobera koren, vpravo nic neni po kolizii\n");
          }
          
          *pole[j]->uzivatelia = *pole[j]->uzivatelia->l;
          return;
        }
        else if (strcmp(pole[j]->uzivatelia->meno, user) == 0) {
          if (debug) {
            printf("Odobera koren, ma obidvoch potomkov po kolizii\n");
          }
          
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
            else { *pole[j]->uzivatelia->l = *pom->l; }
          }
          else {
            strcpy(pole[j]->uzivatelia->meno, pom->meno);

            if (pom->l == NULL) {
              pred->p = NULL;
              pred->pocetp = 0;
            }
            else { pred->p = pom->l; }
          }
        
          return;
        }

        if (debug) {
          printf("Vymazanie ne-korena po kolizii\n");
        }

        odoberUzivatela(pole[j]->uzivatelia, user);
      }

      return;
    }
  }
  // Ak stranka existuje (bez kolizie)
  else if (strcmp(pole[hf]->meno, page) == 0) {
    // Ak je odoberany prvok koren
    if ((strcmp(pole[hf]->uzivatelia->meno, user) == 0) && (pole[hf]->uzivatelia->l == NULL) && (pole[hf]->uzivatelia->p == NULL)){
      if (debug) {
        printf("Vymazanie posledneho (jedineho) like-u\n");
      }

      pole[hf] = NULL;
      return;
    }
    else if ((strcmp(pole[hf]->uzivatelia->meno, user) == 0) && (pole[hf]->uzivatelia->l == NULL)){
      if (debug) {
        printf("Odobera koren, vlavo nic neni\n");
      }

      *pole[hf]->uzivatelia = *pole[hf]->uzivatelia->p;
      return;
    }
    else if ((strcmp(pole[hf]->uzivatelia->meno, user) == 0) && (pole[hf]->uzivatelia->p == NULL)){
      if (debug) {
        printf("Odobera koren, vpravo nic neni\n");
      }

      *pole[hf]->uzivatelia = *pole[hf]->uzivatelia->l;
      return;
    }
    else if (strcmp(pole[hf]->uzivatelia->meno, user) == 0) {
      if (debug) {
        printf("Odobera koren, ma obidvoch potomkov\n");
      }
      
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
        else { *pole[hf]->uzivatelia->l = *pom->l; }
      }
      else {
        strcpy(pole[hf]->uzivatelia->meno, pom->meno);

        if (pom->l == NULL) {
          pred->p = NULL;
          pred->pocetp = 0;
        }
        else { pred->p = pom->l; }
      }
      
      return;
    }

    if (debug) {
      printf("Vymazanie ne-korena\n");
    }

    odoberUzivatela(pole[hf]->uzivatelia, user);
  }
  // Ak nastane kolizia
  else {
    j = hf + 1;

    // Zisti ci stranka uz existuje
    while (j != hf) {
      if ((pole[j] != NULL) && (strcmp(pole[j]->meno, page) == 0)) {
        break;
      }
      
      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }

    if (debug) {
      printf("kolizia, j: %d\n", j);
    }

    if ((pole[j] == NULL) || (strcmp(pole[j]->meno, page) != 0)) {
      if (debug) {
        printf("Stranka neexistuje po kolizii\n");
      }

      return;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      if (debug) {
        printf("Stranka uz existuje po kolizii\n");
      }

      if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->l == NULL) && (pole[j]->uzivatelia->p == NULL)) {
        if (debug) {
          printf("Vymazanie posledneho (jedineho) like-u po kolizii\n");
        }

        pole[j] = NULL;
        return;
      }
      else if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->l == NULL)){
        if (debug) {
          printf("Odobera koren, vlavo nic neni po kolizii\n");
        }

        *pole[j]->uzivatelia = *pole[j]->uzivatelia->p;
        return;
      }
      else if ((strcmp(pole[j]->uzivatelia->meno, user) == 0) && (pole[j]->uzivatelia->p == NULL)){
        if (debug) {
          printf("Odobera koren, vpravo nic neni po kolizii\n");
        }

        *pole[j]->uzivatelia = *pole[j]->uzivatelia->l;
        return;
      }
      else if (strcmp(pole[j]->uzivatelia->meno, user) == 0) {
        if (debug) {
          printf("Odobera koren, ma obidvoch potomkov po kolizii\n");
        }
        
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
          else { *pole[j]->uzivatelia->l = *pom->l; }
        }
        else {
          strcpy(pole[j]->uzivatelia->meno, pom->meno);

          if (pom->l == NULL) {
            pred->p = NULL;
            pred->pocetp = 0;
          }
          else { pred->p = pom->l; }
        }
        
        return;
      }

      if (debug) {
        printf("Vymazanie ne-korena po kolizii\n");
      }

      odoberUzivatela(pole[j]->uzivatelia, user);
    }
  }
}

char *getuser(char *page, int k) {
  if (debug) {
    printf("GETUSER\n");
    printf("page: %s\n", page);
    printf("k: %d\n", k);
  }

  int j, hf = hashFunc(page);
  
  // Ak stranka neexistuje alebo nastane
  // kolizia s neexistujucou (NULL) strankou
  if (pole[hf] == NULL) {
    if (hf == (velk_pola - 1)) { j = 0; }
    else { j = hf + 1; }

    while (j != hf) {
      if ((pole[j] != NULL) && (strcmp(pole[j]->meno, page) == 0)) {
        break;
      }

      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }

    if (pole[j] == NULL) {
      if (debug) {
        printf("Stranka neexistuje\n");
      }

      return NULL;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      if (debug) {
        printf("kolizia, j: %d\n", j);
        printf("Stranka uz existuje po kolizii\n");
      }
      
      return vratUzivatela(pole[j]->uzivatelia, k);
    }

    return NULL;
  }
  // Ak stranka existuje (bez kolizie)
  else if (strcmp(pole[hf]->meno, page) == 0) {
    if (debug) {
      printf("Stranka uz existuje\n");
    }

    return vratUzivatela(pole[hf]->uzivatelia, k);
  }
  // Ak nastane kolizia
  else {
    j = hf + 1;

    // Zisti ci stranka uz existuje
    while (j != hf) {
      if ((pole[j] != NULL) && (strcmp(pole[j]->meno, page) == 0)) {
        break;
      }
      
      if (j == (velk_pola - 1)) { j = 0; }
      else { j++; }
    }

    if ((pole[j] == NULL) || (strcmp(pole[j]->meno, page) != 0)) {
      if (debug) {
        printf("Stranka neexistuje po kolizii\n");
      }

      return NULL;
    }
    else if (strcmp(pole[j]->meno, page) == 0) {
      if (debug) {
        printf("kolizia, j: %d\n", j);
        printf("Stranka uz existuje po kolizii\n");
      }
      
      return vratUzivatela(pole[j]->uzivatelia, k);
    }
  }

  return NULL;
}
            
// Tlacidlo 'Spustit' vyskusa vasu socialnu siet na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.
// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main() {
  init();
  like("Star Trek", "Jana");
  like("Star Wars", "Marek");
  like("Star Trek", "Filip");
  like("Star Trek", "Adam");
  printf("%s\n", getuser("Star Trek", 1)); // Adam
  printf("%s\n", getuser("Star Trek", 2)); // Filip
  unlike("Star Trek", "Filip");
  printf("%s\n", getuser("Star Trek", 1)); // Adam
  unlike("Star Trek", "Jana");

  if (getuser("Star Trek", 1) == NULL) {
    printf("NULL\n");
  }
  else {
    printf("%s\n", getuser("Star Trek", 1));
  }

  printf("%s\n", getuser("Star Wars", 1)); // Marek
  return 0;
} 
