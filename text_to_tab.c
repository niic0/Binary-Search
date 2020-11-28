#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "text_to_tab.h"

void text_to_tab (char *text) {
  FILE *F = fopen(text,"r");
  FILE *tmp = fopen(text,"r");

  if (!F || !tmp) {
    printf("\nFichier inéxistant \n\n");
    exit(2);
  }

  struct stat buf;
	stat(text, &buf);
  int nbr_char = buf.st_size;

  int nbr_lignes = 1;
  int k = 0;
  while (k<=nbr_char) {
    if (fgetc(tmp) == '\n') nbr_lignes++;
    k++;
  }

  fclose(tmp);

  char *T = malloc(nbr_char*(sizeof(char)));
  int *L = malloc(nbr_lignes*(sizeof(int)));

  if (!L || !T) {printf("Pas d'allocation"); exit(4);}

  int i = 0;
  int y = 1;

  while (i<=nbr_char) {
    T[i] = fgetc(F);
    if(T[i] == '\n') {
      L[y] = i+1;
      y++;
    }
    i++;
  }
  fclose(F);
}
