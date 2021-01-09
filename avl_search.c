#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "chrono.h"
#include "avl_fct.h"
#include "affiche_avl.h"
#include "text_to_tab.h" // Pour les messages d'erreurs

AVL *search (AVL* A, char* mot_a_chercher) {
  AVL* result = A; // AVL courant qui va stocker le résultat

  if (result && strcmp(result->mot, mot_a_chercher)) {
    if(strcmp(result->mot, mot_a_chercher) < 0)
      result = search(result->fd, mot_a_chercher);
    else
      result = search(result->fg, mot_a_chercher);
  }

  if (!A) printf("mot non trouvé\n");

  return result;
}

AVL *insert_mot (AVL* A, char *mot, int ligne) {
  // Si l'ABR est vide, il doit être initialisé (malloc)
  if (A == NULL){
    A = cree_noeud(mot);
    A->ligne[0] = ligne;
    return A;
  }

  // L'ABR n'est pas vide
  if(strcmp(mot,A->mot) > 0)           // Si mot à insérer > mot racine
    A->fd = insert_mot(A->fd,mot, ligne);
  else if (strcmp(mot,A->mot) < 0)     // Si mot à insérer < mot racine
    A->fg = insert_mot(A->fg,mot, ligne);

  // Si le mot existe déjà on insère pas et on ajoute 1 d'occurence
  else if (strcmp(mot,A->mot) == 0){
    A->occ++;
    A->ligne = realloc(A->ligne, (A->occ+1)*sizeof(int)); // On allour un emplacement en plus
    if(A->ligne == NULL) allocation_erreur();
    A->ligne[A->occ-1] = ligne;           // On affecte la ligne à la dernière case
    // Sachant que le mot est déjà alloué pour sa première apparition, les cases seront
    // remplies seulement si le mot apparait plusieurs fois
  }

  // On equilibre l'arbre
  A = equilibre_noeud(A);

  return A;
}

// Fonction qui traduit du texte et le met sous forme d'AVL
AVL *tab_to_AVL (AVL* A, char *fic) {
  chrono_reset();
  char *T = char_to_tab(fic);
  printf("\nInitialisation : %f s\n", chrono_lap());

  int nbr_char = taille_fic(fic);

  chrono_reset();

  char *mot_tmp = calloc(64, sizeof(char));   // Buffer
  char *mot_abr;                              // mot à mettre dans l'arbre

  int ligne_actuelle = 1;   // Repère de ligne à insérer dans le mot

  for (int i=0 ; i<=nbr_char ; i++) {
    if ((isspace(T[i]) || ispunct(T[i])) && isalpha(T[i-1])) {
      strcat(mot_tmp,"\0");
      mot_abr = malloc((strlen(mot_tmp)+1)*sizeof(char));
      strcpy(mot_abr,mot_tmp);
      memset(mot_tmp, 0, 64*sizeof(char));
      A = insert_mot(A, mot_abr, ligne_actuelle);
    }
    else if(isalpha(T[i])) {
      T[i] = tolower(T[i]);
      strncat(mot_tmp, &T[i], 1);
    }
    // Si retour à la ligne on passe à la suivante
    if(T[i] == '\n')
      ligne_actuelle++;
  }

  printf("Construction AVL : %f s\n", chrono_lap());

  free(T);
  free(mot_tmp);
  free(mot_abr);
  
  return A;
}
