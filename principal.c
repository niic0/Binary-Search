#include <stdio.h>
#include <stdlib.h>
#include "text_to_tab.h"

int main (int argc, char *argv[]) {
  char* T = text_to_tab(argv[1]);
  int* L = first_char(argv[1]);

  for (int i = 0 ; i<100 ; i++) {
    printf("%5d",L[i]);
  }

  for (int y=0 ; y<100 ; y++) {
    printf("%c",T[y]);
  }
  return 0;
}
