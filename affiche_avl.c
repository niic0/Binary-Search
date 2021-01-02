#include <stdio.h>
#include <stddef.h>
#include "avl_fct.h"

void padding ( char ch, int n )
{
  int i;

  for ( i = 0; i < n; i++ )
    putchar ( ch );
}

void structure (AVL* root, int level )
{
  if ( root == NULL ) {
    padding ( '\t', level );
    puts ( "~" );
  }
  else {
    structure ( root->fd, level + 1 );
    padding ( '\t', level );
    printf ( "%s\n", root->mot );
    structure ( root->fg, level + 1 );
  }
}
