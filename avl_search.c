#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "avl_fct.h"
#include "affiche_avl.h"
#include "text_to_tab.h" // Pour les messages d'erreurs

AVL *search (AVL* A, char* mot_a_chercher) {
  AVL* result = A; // AVL courant qui va stocker le résultat

  if (result && (strcmp(result->mot, mot_a_chercher)!=0)) {
    if(strcmp(result->mot, mot_a_chercher) < 0)
      result = search(result->fd, mot_a_chercher);
    else
      result = search(result->fg, mot_a_chercher);
  }
  if (!A) printf("mot non trouvé\n");
  return result;
}

AVL *insert_mot (AVL* A, char *mot) {

  // L'ABR est vide, il doit être initialisé (malloc)
  if (A == NULL) return cree_noeud(mot);

  // L'ABR n'est pas vide
  if(strcmp(mot,A->mot) > 0){           // Si mot à insérer > mot racine
    A->fd = insert_mot(A->fd,mot);
  }
  else if (strcmp(mot,A->mot) < 0) {   // Si mot à insérer < mot racine
    A->fg = insert_mot(A->fg,mot);
  }
  // Si le mot existe déjà on insère pas et on ajoute 1 d'occurence
  else if (strcmp(mot,A->mot) == 0){
    A->occ++;
    return A;
  }

  // On equilibre l'arbre
  A = equilibre(A);

  return A;
}

    ///////////////////////
    ////// PAS FINIT //////
    ///////////////////////

AVL *tab_to_AVL (AVL* A, char *fic) {
  char *T = char_to_tab(fic);
  int *L = first_char (fic);
  int nbr_char = taille_fic(fic);

  char *mot_tmp = calloc(1, sizeof(char));   // Buffer
  char *mot_abr;                             // mot à mettre dans l'arbre
  int ligne_actuelle = L[0];

  for (int i=0 ; i<=nbr_char ; i++) {

    if ((isspace(T[i]) || ispunct(T[i])) && isalnum(T[i-1])) {
      mot_abr = malloc(strlen(mot_tmp)*sizeof(char));
      strcpy(mot_abr,mot_tmp);
      memset(mot_tmp, 0, sizeof(char));

      A = insert_mot(A,mot_abr);

      if(L[i] == T[i]) ligne_actuelle++;
    }
    else if(isalnum(T[i])) {
      T[i] = tolower(T[i]);
      strncat(mot_tmp, &T[i], 1);
    }
  }

  return A;
}
