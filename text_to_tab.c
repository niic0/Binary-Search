#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "text_to_tab.h"

void verif_fic(char *fic) {
  if (access(fic, F_OK)) {
    printf("\nFichier inéxistant / Erreur ouverture \n\n");
    exit(EXIT_FAILURE);
  }
}

void allocation_erreur () {
  printf("\nAllocation échouée\n\n");
  exit(EXIT_FAILURE);
}

int taille_fic (char *fic) {
  FILE *F = fopen(fic,"r");
  verif_fic(fic);

  struct stat buf;
  stat(fic, &buf);
  int nbr_char = buf.st_size;

  fclose(F);
  return nbr_char;
}

// Compte le nombre de lignes
int cmt_lignes (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic (text);
  int nbr_lignes = 1;

  for (int i=0 ; i<=nbr_char ; i++)
    if (fgetc(F) == '\n') nbr_lignes++;

  fclose(F);
  return nbr_lignes;
}

// Mettre tous les caractères dans un tableau
char* char_to_tab (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic(text);

  char *T = malloc(nbr_char*(sizeof(char)));
  if(T == NULL) allocation_erreur();

  for (int i=0 ; i<=nbr_char ; i++) T[i] = fgetc(F);

  fclose(F);
  return T;
}

//
int* first_char (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic(text);
  int nbr_lignes = cmt_lignes(text);

  int *L = malloc(nbr_lignes*(sizeof(int)));
  if(L == NULL) allocation_erreur();

  int ligne = 1;

  for (int i=0 ; i<=nbr_char ; i++) {
    if(fgetc(F) == '\n') {
      L[ligne] = i+1;
      ligne++;
    }
  }

  fclose(F);
  return L;
}
