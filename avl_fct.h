typedef struct avl_mots {
  int* ligne;     // Première ligne ou le mot apparait: ligne[0], deuxième ligne[1]
  int occ;
  char* mot;
  struct avl_mots *fg, *fd;
} AVL;

AVL *cree_noeud (char* mot);
int AVL_height (AVL *A);
int calcul_desequilibre (AVL *A);
AVL *rotation_gauche (AVL *A);
AVL *rotation_droite (AVL *A);
AVL *equilibre (AVL *A);
void libere_memoire(AVL *A);
void affichage_avl (AVL* A, char* text);
