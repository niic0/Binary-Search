# Projet Langage C

> Projet d'IN301 - Franck Quessette
>
> Projet fait par Ines Lebib et Nicolas Fond-Massany

## Introduction

Ce README sert à expliquer le plus précisément possible le code du projet. Tous les codes sont dans le répertoire github. Le code est ici repris partie par partie avec des annotations ainsi qu'une description détaillée. Ce projet est sur Github pour simplifier les échanges de codes entre binôme et pour mieux prendre en main cet outil. 

## Canevas 

Pour commencer, on doit instaurer un cadre qui nous permettra de créer les éléments utiles aux projet. On créer donc un Makefile, principal.c, text_to_tab.c, text_tot_tab.h, manip_ABR.c, manip_ABR.h, recherche.c. Les fichiers permettant de chronométré notre programme sont donnés avec le sujet du projet.  On peut faire, une fois le terminal dans le répertoire voulue :

` touch Makefile, principal.c text_to_tab.c text_tot_tab.h manip_ABR.c manip_ABR.h recherche.c `

## Du .txt au tableau

Le but de cette partie est de mettre des caractères dans un tableau de caractères et de savoir quelle est le premier caractère de chaque ligne.
On va créer 2 tableaux: ```T[ ]``` de type *char* qui contiendra tous les caractères du fichier et ```L[ ]``` de type *int* qui contiendra la position du premier caractère de chaque ligne de façon à savoir que, par exemple, c'est le caractère 243 qui commence la ligne 4. On notera alors `L[4] = 243`.

Pour simplifier la lecture du code et mieux le comprendre, on découpera correctement le code en fonctions.

On va alors créer **2 fonctions principales**: 

* `*first_char(char *text)` qui va nous permettre d'associer les lignes et le premier caractère qu'elle contient. Elle renverra la variable *int* L décrit plus haut.
* `*text_to_tab(char *text)` qui va nous permettre de mettre dans un tableau de caractère tout le texte. Elle renverra la variable *char* T décrit plus haut.

Ces 2 fonctions ont une étoile ***** car elle renvoie un pointeur qui va nous permettre de reprendre le résultat mallocé des fonctions pour les réutiliser, notamment dans notre fichier principal.c .

### *int* first_char(char *text)

Cette fonction va se servir de plusieurs autre fonctions que nous allons décrire au fur et à mesure du besoin de celle ci. 

Premièrement, on doit ouvrir le fichier. Pour cela on utilise `fopen` de la librairie *stdio.h*. On vérifie ensuite si le fichier à bien été ouvert. Comme on aura souvent besoin de savoir si notre fichier est bien ouvert, on va créer une fonction `void verif_fic(char *fic)` qui arrêtera le programme au cas où le fichier n'existe pas ou que le programme n'est pas les droits d'accéder à ce fichier. 

***

#### *void* verif_fic(char *fic)

On utilise alors "access" de la librairie *unistd* qui permet de vérifier si le fichier existe et si on a les droits pour l'ouvrir en mode écriture. C'est une méthode plus "propre" pour savoir si un fichier et existant ou non. On entrera alors: `if (access(text, F_OK))` et on stoppera le programme s'il rencontre cette erreur. On aurait très bien put mettre un simple `if(!F)`, cela fonctionne aussi. 

```c
void verif_fic(char *fic) {
  if (access(fic, F_OK)) {
    printf("\nFichier inéxistant / Erreur ouverture \n\n");
    exit(EXIT_FAILURE);
  }
}
```

***

Après avoir ouvert notre fichier en mode *read* et vérifier que notre fichier était bien présent dans le dossier où le programme se trouve, on va chercher à mettre dans notre variable *L* les premier caractère de chaque ligne. Cela revient à prendre en compte les caractères juste après un retour à la ligne. Cela se traduit en langage C par `if(fgetc(F) == '\n')`. Mais avant cela on doit allouer notre mémoire pour créer le tableau *L*. On fait pour ça appel à la fonction `malloc` de la librairie *stdlib*. Comme on veux avoir un tableau d'une taille égale aux nombres de lignes du texte, on va faire `int *L = malloc(nbr_lignes*(sizeof(int)));` . Là encore, on a besoin d'une fonction qui nous permettrait de connaître le nombre de ligne du texte. On va alors créer la fonction `int cmt_lignes (char *text)` qui va compter (cmt) les lignes du texte.

***

#### *int* cmt_lignes (char *text)

Pour compter les lignes, on a besoin de lire notre fichier et de simplement compter le nombre de `\n` qui correspond au retour chariot. 

On ouvre donc notre fichier : `FILE *F = fopen(text,"r");` puis on vérifie qu'il soit bien ouvert : `verif_fic(text);`  . On cherche à faire une boucle qui compte le nombre de `\n` dans ce fichier. Pour faire une boucle, il faut savoir où s’arrêter, c'est pourquoi il y a 2 principales manières de faire. 

Soit on attend que `fgetc` nous donne *EOF* qui correspond à la fin du fichier. Cela veux dire qu'on a 2 variables FILE, une pour balayer le fichier avec fgetc et une autre pour vérifier si il y a un `\n` aussi avec fgetc, d'où l'utilité des 2 variables FILE. En effet un appel à fgetc fera passer au caractère suivant à chaque appel, on a donc besoin de 2 variables FILE pour cette méthode.

Soit on balaye le fichier de 0 jusqu’au dernier caractère. Pour cela on a besoin du nombre de caractères dans le fichier. Comme on va avoir à plusieurs reprise besoin d'une fonction permettant de connaître le nombre de caractères, on va utiliser cette fonction. La fonction `taille_fic` sera décrite plus bas.

On va parcourir notre fichier à la recherche de `\n` du caractère 0 jusqu'au dernier caractère. 

```c
int cmt_lignes (char *text) {			// Compte les lignes
  FILE *F = fopen(text,"r");
  verif_fic(text);						// Vérifie si le fichier éxiste
  
  int nbr_char = taille_fic(text);		// Nombre de caractères
  int nbr_lignes = 1;					// On commence à la ligne 1

  for (int i=0 ; i<=nbr_char ; i++) {
    if (fgetc(F) == '\n') nbr_lignes++;
  }

  fclose(F);							// On ferme le fichier
  return nbr_lignes;					// On retourne le nombre de lignes		
}
```

***

***

#### *int* taille_fic(char *text)

On veux dans cette fonction retourner la taille d'un fichier, ce qui revient à compter le nombre de caractères de celui ci. Pour ça on va utiliser la fonction donnée avec le projet. Cette manière utilise la librairie *stat.h* (qui peut d'ailleurs se trouver dans *sys/stat.h* en fonction des OS). Toujours la même chose, on va ouvrir notre fichier,  vérifier s'il est bien ouvert (renvoyer une erreur sinon) faire le corps principal de notre fonction puis fermer le fichier et enfin return la taille de fichier (égale au nombre de caractères). Pour plus de détail, il faut aller voir le manuel de la fonction *stat*. 

```c
int taille_fic (char *fic) {
  FILE *F = fopen(fic,"r");
  verif_fic(fic);

  struct stat buf;
  stat(fic, &buf);
  int nbr_char = buf.st_size;

  fclose(F);
  return nbr_char;
}
```

***

Revenons à notre code dans sa globalité. On a maintenant, le nombre de caractères du texte, le nombre de lignes et l'allocation mémoire de *L (alloué autant de fois qu'il y a de lignes). On va maintenant passer à la boucle qui va permettre de faire correspondre chaque caractère de début de ligne à sa ligne.

On va pour ça faire une boucle qui balayera le texte et qui va, pour chaque `\n` indiquer le caractère du début de ligne et le mettre dans le tableau *L[y]*, *y* étant la ligne correspondante. 

**Programme final de *first_char* **

```c
int *first_char (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic(text);
  int nbr_lignes = cmt_lignes(text);

  int *L = malloc(nbr_lignes*(sizeof(int)));
  if(L == NULL) affiche_erreur();

  int y = 1;

  for (int i=0 ; i<=nbr_char ; i++) {	// Parcours du texte
    if(fgetc(F) == '\n') {				// Si il y a un retour à la ligne
      L[y] = i+1;						// Le caractère suivant = 1er char de la ligne
      y++;								// On avance d'un ligne
    }
  }

  fclose(F);
  return L;
}
```

### *char* text_to_tab(char *text)

Cette fonction sera beaucoup moins longue que la précédente puisqu'elle utilisera toute les fonctions vu précédemment (à l'exception de *first_char* évidement). 

Premièrement on ouvre le fichier puis on vérifie s'il est ouvert. Pour ça toujours la méthode et la même fonction `verif_fic`. Ensuite, on va chercher à mettre chaque caractères dans le tableau. Pour ça on va parcourir le fichier avec une boucle encore une fois en utilisant fgetc. Cette fonction trouve en effet très bien sa place ici puisqu'elle va nous permettre de mettre dans *T[ i ]* tous les caractères. 

Pour ça on va avoir besoin d'une variable nbr_char qui aura pour fonction à la fois de balayer le texte avec une boucle mais aussi d'allouer la bonne capacité de mémoire pour notre variable T. On met alors dans une variable *nbr_char* la taille du fichier avec notre fonction `taille_fic`. On alloue ensuite la mémoire de T en faisant `char *T = malloc(nbr_char*(sizeof(char)));` . On vérifie si la mémoire est bien alloué. On fait un for qui va permettre de parcourir le mot et d'implémenter chaque caractère du texte dans notre tableau.

Finalement on ferme le fichier puis on retourne T.

```c
char *text_to_tab (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic(text);

  char *T = malloc(nbr_char*(sizeof(char)));
  if(T == NULL) affiche_erreur();

  for (int i=0 ; i<=nbr_char ; i++) T[i] = fgetc(F);

  fclose(F);
  return T;
}
```



