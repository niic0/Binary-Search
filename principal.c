#include <stdio.h>
#include <stdlib.h>
#include "text_to_tab.h"
#include "avl_fct.h"
#include "avl_search.h"
#include "affiche_avl.h"

int main (int argc, char *argv[]){
  AVL *A = NULL;
  AVL *noeud_a_trouver = NULL;

  A = tab_to_AVL(A,argv[1]);
  structure(A,0);
  noeud_a_trouver = search(A,"b");
  printf("Structure du noeud trouvé\n");
  structure(noeud_a_trouver,0);
//  affichage_avl(noeud_a_trouver,argv[1]);

  libere_memoire(A);

  return 0;
}
