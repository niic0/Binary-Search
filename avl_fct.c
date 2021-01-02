#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_fct.h"
#include "affiche_avl.h"
#include "text_to_tab.h" // Pour les messages d'erreurs

// Initialise un noeud
AVL *cree_noeud (char* mot) {
  AVL* A = NULL;
  A = malloc(sizeof(AVL));
  if (A == NULL) allocation_erreur();

  A->fg = NULL; A->fd = NULL;
  A->occ = 1;

  A->mot = malloc(strlen(mot)*sizeof(char*));
  if (A->mot == NULL) allocation_erreur();

  A->mot = mot;

  return A;
}

// Libère la mémoire
void libere_memoire(AVL *A){
  if(A){
    libere_memoire(A->fg);
    libere_memoire(A->fd);
    free(A);
  }
}

// Trouve la hauteur d'un AVL récusrivement
int AVL_height (AVL *A) {
  int height_g = 0, height_d = 0;   // fils gauche, fils droit

  if (A->fg) height_g = AVL_height(A->fg);
  if (A->fd) height_d = AVL_height(A->fd);

  if (height_d > height_g) return ++height_d; // On incrémente avant le return
  else return ++height_g;                     // pour qu'il prenne la bonne val
}

// Calul le déséquilibre d'un ABR
int calcul_desequilibre (AVL *A) {
  int delta = 0;        // Variable qui stock le déséquilibre

  if(!A) return 0;

  if(A->fd) delta += AVL_height(A->fd);
  if(A->fg) delta -= AVL_height(A->fg);

  return delta;
}

    /////////////////
    /// Rotations ///
    /////////////////

AVL *rotation_droite (AVL *A) {
  AVL* result = NULL;

  AVL* tmp = A;
  result = A->fg;

  tmp->fg = result->fd;
  result->fd = tmp;

  return result;
}

AVL *rotation_gauche (AVL *A) {
  AVL* result = NULL;

  AVL* tmp    = A;
  result = tmp->fd;

  tmp->fd    = result->fg;
  result->fg = tmp;

  return result;
}

// Equillibre l'arbre pour qu'il devienne un AVL
AVL *equilibre (AVL *A) {
  AVL *result = cree_noeud(A->mot);

  // On équilibre chaque fils
  if(A->fg) A->fg = equilibre(A->fg);
  if(A->fd) A->fd = equilibre(A->fd);

  int diff = calcul_desequilibre(A); // diff = déséquilibre

  if (diff >= 2) {
    if (calcul_desequilibre(A->fd) <= -1) {
      result->fd = rotation_droite(A->fd);
      result = rotation_gauche(result);
    }
    else if (calcul_desequilibre(A->fd) >= 1) {
      result = rotation_gauche(A);
    }
  }

  else if (diff <= -2) {
    if (calcul_desequilibre(A->fg) >= 1) {
      result->fg = rotation_gauche(A->fg);
      result = rotation_droite(result);
    }
    else if (calcul_desequilibre(A->fg) <= -1) {
      result = rotation_droite(A);
    }
  }

  if(diff>-2 && diff<2) result = A;  // Si rien à faire A est équilibré

  return result;
}

          /////////////////
          /// Affichage ///
          /////////////////

void affichage_avl (AVL* A, char* text) {
  char* T = char_to_tab (text);
  int* L = first_char(text);

  printf("Occurence du mot %s : %d\n",A->mot,A->occ);
  for (int i=1 ; i<A->occ ; i++) {
    printf("ligne %d :\n",A->ligne[i]);
    for (int k=L[A->ligne[i]] ; k<L[A->ligne[i+1]] ; k++)
      printf("%c\n",T[i]);
  }
}

void affichage_chrono (AVL* A) {

}
