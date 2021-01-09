#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chrono.h"
#include "text_to_tab.h"
#include "avl_fct.h"
#include "avl_search.h"

int main (int argc, char *argv[]){
  AVL *A = NULL;
  AVL *noeud_a_trouver = NULL;

  A = tab_to_AVL(A,argv[1]);

  // Si pas d'arguments
  if(!argv[2]) {
    char* mot;
    srand(getpid());
    chrono_reset();

    for (int i=0 ; i<1000 ; i++) {
      mot = mot_alea(A);
      noeud_a_trouver = search(A,mot);
      printf("%s\n",mot);
    }

    printf("Recherche pour 1000 mots : %f s\n\n", chrono_lap());
  }

  // S'il y a des arguments
  for(int k=2 ; k<argc ; k++) {  // On commence par 2 pour obtenir le/les mots pris en, argument
    chrono_reset();
    noeud_a_trouver = search(A,argv[k]);
    affichage_avl(noeud_a_trouver,argv[1]);
    printf("\nTemps de recherche du mot %s : %f s\n\n",argv[k], chrono_lap());
  }

  libere_memoire(A);
  return 0;
}
