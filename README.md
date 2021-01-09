# Projet Langage C

> Projet d'IN301 - UVSQ
>

## Introduction

Ce README sert à expliquer le plus précisément possible le code du projet. Tous les codes sont dans le répertoire github. Le code est ici repris partie par partie avec des annotations ainsi qu'une description. Ce projet est sur Github pour simplifier les échanges de codes entre binôme.

## Canevas

Pour commencer, on doit instaurer un cadre qui nous permettra de créer les éléments utiles au projet. On créer un Makefile, principal.c, text_to_tab.c, text_to_tab.h, avl_fct.c, avl_fct.h, avl_search.c et avl_search.h. Les fichiers permettant de chronométrer notre programme sont donnés avec le sujet du projet.  On peut faire, une fois le terminal dans le répertoire voulue :

` touch Makefile principal.c text_to_tab.c text_to_tab.h avl_fct.c avl_fct.h avl_search.c avl_search.h`

## Du .txt au tableau

Le but de cette partie est de mettre des caractères dans un tableau de caractères et de savoir quelle est le premier caractère de chaque lignes.
On va créer 2 tableaux: ```T[ ]``` de type *char* qui contiendra tous les caractères du fichier et ```L[ ]``` de type *int* qui contiendra la position du premier caractère de chaque ligne de façon à savoir que, par exemple, c'est le caractère 243 qui commence la ligne 4. On notera alors `L[4] = 243`.

Pour simplifier la lecture du code et mieux le comprendre, on découpera correctement le code en plusieurs fonctions.

On va alors créer **2 fonctions principales**:

* `char *first_char(char *text)` qui va nous permettre d'associer les lignes et le premier caractère qu'elle contient. Elle renverra la variable *int* L décrit plus haut.
* `char *char_to_tab(char *text)` qui va nous permettre de mettre dans un tableau de caractères tout le texte. Elle renverra la variable *char* T décrit plus haut.

Ces 2 fonctions ont une étoile ***** car elle renvoie un pointeur qui va nous permettre de reprendre le résultat alloué des fonctions pour les réutiliser, notamment dans notre fichier principal.c .

### *int* first_char(char *text)

Cette fonction va se servir de plusieurs autre fonctions que nous allons décrire au fur et à mesure du besoin de celle ci.

Premièrement, on doit ouvrir le fichier. Pour cela on utilise `fopen` de la librairie *stdio.h*. On vérifie ensuite si le fichier à bien été ouvert. Comme on aura souvent besoin de savoir si notre fichier est bien ouvert, on va créer une fonction `void verif_fic(char *fic)` qui arrêtera le programme au cas où le fichier n'existe pas ou que le programme n'est pas les droits d'accéder à ce fichier.

***

#### *void* verif_fic(char *fic)

On utilise alors *access* du header *unistd.h* qui permet de vérifier si le fichier existe et si on a les droits pour l'ouvrir en mode écriture. C'est une méthode plus "propre" pour savoir si un fichier et existant ou non. On entrera alors: `if (access(text, F_OK))` et on stoppera le programme s'il rencontre cette erreur. On aurait très bien put mettre un simple `if(!F)`, cela fonctionne aussi.

```c
void verif_fic(char *fic) {
  if (access(fic, F_OK)) {		// Vérification de l'ouverture et des droits sur le fichier
    printf("\nFichier inéxistant / Erreur ouverture \n\n");	// Message d'erreur si erreur(s)
    exit(EXIT_FAILURE);			// On sort du programme si erreur(s)
  }
}
```

***

Après avoir ouvert notre fichier en mode *read* et vérifié que notre fichier était bien présent dans le dossier où le programme se trouve, on va chercher à mettre dans notre variable *L* les premier caractère de chaque lignes. Cela revient à prendre en compte les caractères juste après un retour à la ligne. Cela se traduit en langage C par `if(fgetc(F) == '\n')`. Mais avant cela on doit allouer notre mémoire pour créer le tableau *L*. On fait pour ça appel à la fonction `malloc` du header *stdlib.h*. Comme on veux avoir un tableau d'une taille égale aux nombres de lignes du texte, on va faire `int *L = malloc(nbr_lignes*(sizeof(int))+1);` (+1 pour ne pas oublier la première ligne. Là encore, on a besoin d'une fonction qui nous permettrait de connaître le nombre de lignes du texte. On va alors créer la fonction `int cmt_lignes (char *text)` qui va compter (*cmt*) les lignes du texte.

***

#### *int* cmt_lignes (char *text)

Pour compter les lignes, on a besoin de lire notre fichier et de simplement compter le nombre de `\n` qui correspond au retour chariot.

On ouvre donc notre fichier : `FILE *F = fopen(text,"r");` puis on vérifie qu'il soit bien ouvert : `verif_fic(text);`  . On cherche à faire une boucle qui compte le nombre de `\n` dans ce fichier. Pour faire une boucle, il faut savoir où s’arrêter, plusieurs possibilité sont alors possibles (elles ne sont pas toutes cités).

Soit on attend que `fgetc` nous donne *EOF* qui correspond à la fin du fichier. Cela implique 2 variables FILE, une pour balayer le fichier avec `fgetc` et une autre pour vérifier si il y a un `\n` , aussi avec fgetc, d'où l'utilité des 2 variables FILE. En effet un appel à `fgetc` fera passer au caractère suivant à chaque appel, on a donc besoin de 2 variables FILE pour cette méthode.

Soit on balaye le fichier de 0 jusqu’au dernier caractère. Pour cela on a besoin du nombre de caractères dans le fichier. Comme on va avoir à plusieurs reprise besoin d'une fonction permettant de connaître le nombre de caractères, on va utiliser cette fonction. La fonction `taille_fic` sera décrite plus bas.

On va parcourir notre fichier à la recherche de `\n` , du caractère 0 au dernier.

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

On veux dans cette fonction retourner la taille d'un fichier, ce qui revient à compter le nombre de caractères de celui ci. Pour ça on va utiliser la fonction donnée avec le projet. Cette méthode utilise la librairie *stat.h* (qui peut d'ailleurs se trouver dans *sys/stat.h*). Toujours la même chose, on va ouvrir notre fichier,  vérifier s'il est bien ouvert (renvoyer une erreur sinon) faire le corps principal de notre fonction puis fermer le fichier et enfin *return* la taille de fichier (égale au nombre de caractères). Pour plus de détail, il faut aller voir le manuel de la fonction *stat*.

```c
int taille_fic (char *fic) {
  FILE *F = fopen(fic,"r");	// Ouvre le fichier en mode read
  verif_fic(fic);			// Vérifie si le fichier s'est ouvert correctement

  struct stat buf;			// Prend une struct de stat.h
  stat(fic, &buf);			// Calcul de la taille du fichier par la fct stat	
  int nbr_char = buf.st_size;	// Variable nbr_char qui prend la valeur calculée

  fclose(F);				// Fermeture du fichier
  return nbr_char;			// On retourne la variable contenant la taille du fichier
}
```

***

Revenons à notre code dans sa globalité. On a maintenant, le nombre de caractères du texte, le nombre de lignes et l'allocation mémoire de **L* (alloué autant de fois qu'il y a de lignes). On va maintenant passer à la boucle qui va permettre de faire correspondre chaque caractères de début de ligne à sa ligne.

On va pour ça faire une boucle qui balayera le texte et qui va, pour chaque `\n` indiquer le caractère du début de ligne et le mettre dans le tableau *L[ligne]*

**Programme final de *first_char* **

```c
int *first_char (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic(text);
  int nbr_lignes = cmt_lignes(text);

  int *L = malloc(nbr_lignes*(sizeof(int)));
  if(L == NULL) affiche_erreur();
  
  L[1]=0;				// L[1]=0 car la ligne 1 commence forcément pas le caractère 0 
  int ligne = 1; 

  for (int i=0 ; i<=nbr_char ; i++) {		// Parcours du texte
    if(fgetc(F) == '\n') {					// Si il y a un retour à la ligne
      L[ligne+1] = i+1;						// Le caractère suivant = 1er char de la ligne
      ligne++;								// On avance d'un ligne
    }
  }

  fclose(F);
  return L;
}
```

***

### *char*  char_to_tab (char *text)

Cette fonction sera beaucoup moins longue que la précédente puisqu'elle utilisera des fonctions vu précédemment.

Premièrement on ouvre le fichier puis on vérifie s'il c'est correctement ouvert. Pour ça toujours la méthode et la même fonction `verif_fic`. Ensuite, on va chercher à mettre chaque caractères dans le tableau. Pour ça on va parcourir le fichier avec une boucle encore une fois en utilisant *fgetc*. Cette fonction trouve en effet très bien sa place ici puisqu'elle va nous permettre de mettre dans *T[ i ]* tous les caractères.

Pour ça on va avoir besoin d'une variable *nbr_char* qui aura pour fonction à la fois de balayer le texte avec une boucle mais aussi d'allouer la bonne capacité de mémoire pour notre variable T. On met alors dans une variable *nbr_char* la taille du fichier avec notre fonction `taille_fic`. On alloue ensuite la mémoire de T en faisant `char *T = malloc(nbr_char*(sizeof(char)));` . On vérifie si la mémoire est bien alloué. On fait un *for* qui va permettre de parcourir le mot et d'implémenter chaque caractères du texte dans notre tableau.

Finalement on ferme le fichier puis on retourne T.

```c
char* char_to_tab (char *text) {
  FILE *F = fopen(text,"r");
  verif_fic(text);

  int nbr_char = taille_fic(text);

  char *T = malloc(nbr_char*(sizeof(char))+1);
  if(T == NULL) allocation_erreur();

  for (int i=0 ; i<=nbr_char ; i++) T[i] = fgetc(F);	// Chaque char est insérer dans T[i]

  fclose(F);
  return T;
}
```

***

## Fonctions sur les AVL pour des chaînes de caractères

Il a plusieurs manières de créer un AVL sur des *string* (chaîne de caractères). Ici, on ne traitera qu'une seule méthode assez simple à comprendre si on connaît les principes d'un AVL. Que ce soit des *int*, des *float* ou des char, la méthode reste quasiment la même. C'est principalement les structures qui différent. **Cette partie concerne les fonctions permettant de créer cet AVL mais pas de le créer directement, nous verrons ça dans la partie suivant.**

On veux trouver un mot qui se situe dans le texte avec la commande `./dico text mot_à_trouver`. Pour que ce soit efficace, on va construire un arbre binaire de recherche (ABR). La recherche sera alors bien plus rapide car on effectuera moins d'itérations que si on avait à prendre chaque mot du texte et  le comparer au mot à chercher. Pour aller plus vite encore, on va construire un AVL. Un ABR équilibré sera bien plus efficace qu'un ABR basique, on effectuera beaucoup moins d'itérations dans les fonctions. L'optimisation est dans ce projet (et dans tous les programmes) importantes puisqu'en fin de projet, il est demandé de chronométré notre programme, pour la recherche, le temps d’exécution totale et certaines fonctions. 

On va partitionner le code en plusieurs fonctions et plusieurs fichiers. Un fichier qui contient tout ce qu'il faut pour manipuler un AVL et un autre pour chercher dans cet AVL le mot voulue. On mettra enfin tout ça dans le `principal.c` pour que le programme puisse comprendre la commande entrée avec le ou les mot(s) à chercher et le texte dans lequel il faut chercher.

Les fichiers seront nommés `avl_search.c`, `avl_search.h,` `avl_fct.c` et  `avl_fct.h`.

 ### avl_fct.c / avl_fct.h

Toutes les fonctions qui vont nous être utiles dans avl_search sont ici créées. Il n'y aura pas de fonctions de recherche ni d'insertion du tableau T fait précédemment. Pour savoir quelle fonctions on doit créer, on va partir de la définition d'un AVL. Le principe fondamental d'un AVL est qu'il est équilibré. On part donc du fait qu'il puisse admettre maximum **1 de différence** de hauteur dans le sous arbres droit ou gauche de la racine. Pour résumer, un AVL est un ABR H-équilibré avec un déséquilibre de 1 de hauteur maximum dans ces fils. Il nous faudra alors:

* Les fonctions classiques de création d'un ABR : l'initialisation et l'insertion d'un nœud.
* Une fonction qui trouve la hauteur d'un arbre afin de connaître le déséquilibre d'un arbre par le biais d'une autre fonction qui indiquera ce déséquilibre.
* S'il y a un déséquilibre c'est qu'il faut rééquilibré l'arbre. Il nous faudra donc des fonctions qui font des rotations droite gauche, gauche droite. Pour les rotations gauche droite et droite gauche, on utilisera simplement les rotations droite et gauche déjà faites.
* Si on a les fonctions de rotations, il faut une autre fonction capable de déterminer quelle rotation il faut faire. On créera donc une fonction permettant d'équilibrer l'arbre.

Avec ces fonctions, on aura tous les outils en main pour chercher un mot dans notre texte dans le programme `avl_search.c`.

On créer premièrement une structure nous permettant de manipuler simplement l'arbre. On implémente cette structure dans notre `avl_fct.h`.

```c
typedef struct avl_mots {
  int* ligne;     // Première ligne ou le mot apparait: ligne[0], deuxième ligne[1]
  int occ;
  char* mot;
  struct avl_mots *fg, *fd;
} AVL;
```

#### *AVL* creer_avl ()

Cette fonction permet de créer / d'initialiser un AVL. Elle aura pour but d'allouer correctement la mémoire (et d'indiquer s'il y a une erreur dans l'allocation mémoire). Comme on initialise ici les valeurs de notre futurs AVL, on mettra dans chacune d'entre elles des valeurs NULL. La mémoire du mot sera alloué selon la taille du mot, on mallocera la mémoire en temps voulue. On initialise l’occurrence à 1 puisque si on créer un mot, c'est qu'il existe au moins 1 fois dans le texte. Même logique pour les lignes, si on créer un nœud c'est qu'il existe un mot à une ligne donc on créer un tableau de 1 puisqu'on ne connaît pas encore l’occurrence du mot, on ne sait donc pas combien de fois il apparaît et donc combien de mémoire il faut allouer. On verra comment gérer la mémoire de `A->lignes` dans une autre fonction.

```c
AVL *cree_noeud (char* mot) {
  AVL* A = NULL;						// Pointeurs vers NULL
  A = malloc(sizeof(AVL));				// Allocation du pointeurs
  if (A == NULL) allocation_erreur();	// Vérification de l'allocation

  A->fg = NULL; A->fd = NULL;			// les fils sont à NULL
  A->occ = 1;							// L'occurence est a 1
  A->ligne = calloc(1, sizeof(int));	// On allour un tableau de 1 
										// pour le tableau de lignes
  A->mot = mot;
    
  return A;
}
```

 #### *int* height_arbre (AVL A)

Comme dit précédemment cette fonction nous permettra de connaître le déséquilibre d'un arbre dans une autre fonction. La hauteur d'un arbre correspond au nombre maximum de nœuds entre la racine (de l'arbre entier) et une feuille. Il faudra donc comparer le fils droit et gauche de chaque racines afin de savoir lequel est le plus grand. Pour ça on va faire une fonction récursive qui va retourner la plus grande valeur entre le fils droit et le fils gauche. A ce moment là on comparera le fils droit et gauche de chaque sous arbres.
On calculera la hauteur du sous arbre droit et gauche de la racine puis si ils ont 2 fils la hauteur du sous arbre droit et gauche de cette racine et ainsi de suite...

```c
int AVL_height (AVL *A) {
  int height_g = 0, height_d = 0;				// fils gauche, fils droit

  if (A->fg) height_g = AVL_height(A->fg);		// S'il y a un fg on calcul la hauteur du fg
  if (A->fd) height_d = AVL_height(A->fd);		// S'il y a un fd on calcul la hauteur du fd

  if (height_d > height_g) return ++height_d; // On incrémente avant le return
  else return ++height_g;                     // pour qu'il prenne la bonne val
}
```

#### *int* calcul_desequilibre (AVL A)

Cette fonction a pour but de calculer le déséquilibre d'un arbre. Comme on a fait précédemment une fonction qui calcul la hauteur d'un arbre, il ne nous reste plus qu'à ajouter à une variable *int* la hauteur du fils droit et de soustraire cette valeur à la hauteur du fils gauche. On agit sur les variables seulement si elles existent, on va donc mettre un `if (/*variable*/)` .

```c
int calcul_desequilibre(AVL *A) {
  int difference = 0;   				 	// Variable qui stock le déséquilibre

  if(!A) return 0;							// Si l'arbre n'existe pas on retourne 0
    
  if(A->fd) difference += AVL_height(A->fd);	// la difference vaux la hauteur du sous arbre droit  
  if(A->fg) difference -= AVL_height(A->fg);	// On fait la différence avec la hauteur du sous arbre gauche

  return difference;
}
```

#### Les rotations

Les rotations vont nous permettre d'équilibrer l'arbre. Il existe 4 rotations : gauche, droite, droite gauche et gauche droite. Les rotations droite et gauche sont suffisantes pour coder les rotation gauche droite et droite gauche.

##### Rotation gauche et rotation droite

![img](https://upload.wikimedia.org/wikipedia/commons/0/04/Tree_rotation_fr.svg)

> Schéma provenant de Wikipédia https://fr.wikipedia.org/wiki/Rotation_d%27un_arbre_binaire_de_recherche

```c
AVL *rotation_gauche (AVL *A) {
  AVL* result = NULL;			// On initialise notre resultat à NULL
    
  AVL* tmp    = A;				// Notre variable tmp prend l'arbre
  result = tmp->fd;

  tmp->fd    = result->fg;
  result->fg = tmp;

  return result;
}

AVL *rotation_droite (AVL *A) {
  AVL* result = NULL;			// On initialise notre resultat à NULL
    
  AVL* tmp = A;					// Notre variable tmp prend l'arbre
  result = A->fg;

  tmp->fg = result->fd;
  result->fd = tmp;

  return result;
}

```

**Explications**
*Rotation droite*
On détail ici à chaque variables et ce que fait l’affectation :

```
AVL *tmp    = A;
AVL *result = A->fg;

tmp :      Q			result :    P
	  	 /	 \					  /   \		
    	P	  C					 A	   B
      /   \
     A	   B
```

```
tmp->fg    = result->fd;
result->fd = tmp;

tmp :      Q			result :    P
	  	 /	 \					  /   \		
    	B	  C					 A	   Q (tmp)
    								 /	 \
    								B     C
```

*Rotation gauche*

```
AVL *tmp    = A;
AVL *result = A->fd;

tmp :      P			result :    Q
	  	 /	 \					  /   \		
    	A	  Q					 B     C
    		/   \
    	   B	 C
```

```
tmp->fg    = result->fd;
result->fd = tmp;

tmp :      P			result :    Q
	  	 /	 \					  /   \		
    	A	  B			   (tmp) P     C
    						   /   \
                              A     B
```

##### Rotations droite gauche et droite gauche

Les rotations gauche droite et gauche droite sont sensiblement les même à la différence qu'elles sont "inverses" l'une de l'autre. En effet quand on code les 2 algos, on se rend compte qu'il suffit de faire un copier coller et de changer tous les fg (fils gauche) en fd (fils droit) et tous les fd en fg. Pour faire une rotation droite gauche il faut faire une rotation droite sur le fils droit et une rotation gauche sur la racine. Pour faire une rotation gauche droite il faut faire une rotation droite sur le fils gauche et une rotation gauche sur la racine. 

Les rotations gauche droite et droite gauche apparaîtront donc dans la fonction *équilibre*.

Rotation droite gauche

![img](https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/Arbres%20AVL_fichiers/image010.jpg)

Rotation gauche droite

![img](https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/Arbres%20AVL_fichiers/image012.jpg)

> Figures du site https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/arbres%20AVL.htm

#### *AVL* *equilibre (AVL *A)

Cette fonction permet d'équilibré un ABR pour qu'il devienne un AVL. Le but est de parcourir tout l'arbre et de détecter si le déséquilibre vaux 2 ou -2. On sait que :

* Si le nœud a une différence de profondeur de sous-arbre supérieur ou égale à 2 :
  * Si le fils gauche à une différence inférieur ou égale à -1, il faut faire une rotation droit gauche.
  * Si le fils gauche à une différence supérieur ou égale à 1 il faut faire une rotation gauche.
* Si le nœud a une différence de profondeur de sous-arbre inférieur ou égale à -2 :
  * Si le fils droit à une différence inférieur ou égale à -1 il faut faire une rotation droite.
  * Si le fils gauche à une différence supérieur ou égale à 1 il faut faire une rotation gauche droite.

On peut représenter ça sous forme de tableau :

| Différence                  | Équilibre de la racine : 2 | Équilibre de la racine : -2 |
| --------------------------- | -------------------------- | --------------------------- |
| **Équilibre fils gauche 1** | Simple Gauche              | Gauche Droite               |
| **Équilibre fils droit -1** | Droite Gauche              | Simple Droite               |

La fonction est récursive afin d'avoir un code plus simple.

```c
AVL *equilibre_noeud (AVL *A) {

  int diff = calcul_desequilibre(A); // diff = déséquilibre

  if (diff >= 2) {
    if (calcul_desequilibre(A->fd) <= -1) {
      A->fd = rotation_droite(A->fd);
      A = rotation_gauche(A);
    }
    else if (calcul_desequilibre(A->fd) >= 1)
      A = rotation_gauche(A);
  }

  else if (diff <= -2) {
    if (calcul_desequilibre(A->fg) >= 1) {
      A->fg = rotation_gauche(A->fg);
      A = rotation_droite(A);
    }
    else if (calcul_desequilibre(A->fg) <= -1)
      A = rotation_droite(A);
  }

  return A;
}
```

## Recherche dans un AVL
### *AVL* *insert_mot (AVL *A, char *mot)
Cette fonction permet d'insérer un mot dans un arbre vide ou non. Elle nous permettra pas la suite de mettre tous les mots du tableau contenant les caractères.  Cette fonction prend en compte les arbres vides donc pas de prérequis sur l'allocation, la fonction fait tout.
La fonction est assez simple puisque c'est un ABR de string avec l'ajout d'une fonction qui équilibre (devenant AVL).
Pour comparer les chaînes de caractères du mot et du mot de la racine on va utiliser la bibliothèque `<string.h>` pour la fonction `strcmp(mot A, mot B)`. Cette fonction renvoie 0 si les deux mots sont les même (sachant qu'un mot ne doit pas apparaître 2 fois dans l'arbre), si le mot A est plus petit que le mot B une valeur négative sera retournée, si le mot A est plus grand que le mot B une valeur plus grande est retournée. Tout ça au sens lexicale (a<m<z).
Une fois qu'on sait si le mot doit aller à gauche ou à droite de la racine, on vérifie que l'endroit où doit aller le mot n'est pas NULL car si c'est le cas, c'est qu'on a trouver qon emplacement (en tant qu'abr, il risque de changer de place avec l'équilibre). Sinon la fonction s’appelle elle même. En faisant ça, elle parcourt l'arbre jusqu'à trouver un fils NULL où il peux aller.

```c
AVL *insert_mot (AVL* A, char *mot, int ligne) {
  // Si l'ABR est vide, il doit être initialisé (malloc)
  if (A == NULL){
    A = cree_noeud(mot);
    A->ligne[0] = ligne;		// La ligne alloué dans cree_noeud(mot) 
    return A;					// devient la 1ère valeur du tableau
  }

  // L'ABR n'est pas vide
  if(strcmp(mot,A->mot) > 0)           // Si mot à insérer > mot racine
    A->fd = insert_mot(A->fd,mot, ligne);
  else if (strcmp(mot,A->mot) < 0)     // Si mot à insérer < mot racine
    A->fg = insert_mot(A->fg,mot, ligne);

  // Si le mot existe déjà on n'insère pas et on ajoute 1 d'occurence
  else if (strcmp(mot,A->mot) == 0){
    A->occ++;
    A->ligne = realloc(A->ligne, (A->occ+1)*sizeof(int)); // On allour un emplacement en plus
    if(A->ligne == NULL) allocation_erreur();
    A->ligne[A->occ-1] = ligne;     // On affecte la ligne à la dernière case
                                    // Sachant que le mot est déjà alloué pour sa première apparition, les cases seront
                                    // remplies seulement si le mot apparait plusieurs fois
  }
```

### *AVL* *tab_to_AVL (AVL* A, char *fic)

Cette fonction permet de concrétiser toute les autres fonctions. En effet, elle a pour but de prendre le fichier pris en argument et de le retranscrire en AVL (aussi pris en argument). Pour ça, on va mettre bout à bout les fonctions importantes précédemment faites. Les chrono sont uniquement là pour chronométré les performances des parties de la fonction. `chrono_reset()` "enclenche" le chrono et `chrono_lap()` l’arrête. On va chercher à prendre les caractères et à les mettre sous forme de mots. Pour ça on va utiliser les fonctions `isspace`, `ispunct` et `isalpha` qui vont nous permettre respectivement de savoir si le caractère et un espace (un \n, un espace vide...), une ponctuation ('!', '.' , '/' , ...) ou si c'est un therme numérique (1, 2, 3, 4, ...). Si on sait ça on sait que tout ce qui se trouve entre ces caractères non alphabétiques sont des mots. 

La première chose à faire est d'avoir tous les caractères du texte. On créer alors notre tableau qui nous renverra tous les caractères du texte avec la fonction `char_to_tab`. On peut maintenant faire une boucle qui va aller de 0 jusqu'au dernier caractère. Dans cette boucle, on va chercher à sortir chaque mots à partir de la méthode vu au dessus. Si on sait ou se trouve tous les thermes non alphabétiques, on sait que tout ce qui s'y trouve entre est un mot, une chaîne de caractère. 

Pour stocker le mots, on a besoin de 2 pointeurs. Un qui concaténera tous les caractères alphabétiques entrant et un autre qui copiera cette chaîne de caractère pour l'insérer dans notre arbre. On fait ça car s'il on entre directement le mot concaténer dans notre arbre et qu'on change cette chaîne de caractère, la mémoire pointée changera alors et tous les mots seront identiques ce qui peux créer des espaces vides dans notre arbres. La méthode est donc la suivante: On a un pointeur qui concatène les caractères entrants. Une fois qu'on tombe sur autre chose qu'une lettre de l'alphabet on ajoute tout d'abord à notre mot_tmp le caractère de fin de chaîne. On alloue ensuite le mot qui va être inséré dans notre arbre. Finalement on insert le mot dans notre arbre puis on remet à 0 la mémoire du mot temporaire en allouant 64 char dans notre tableau (qui est en fait un buffer) avec `memset` afin qu’il concaténer le prochain mot.

Pour affecter la ligne au nœud, on va compter les lignes avec un simple `if ('\n')`  puis en augmentant de 1 notre variable qui indique la ligne où on se situe. 

```c
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
      strcat(mot_tmp,"\0");		// Ajoute le caractère de fin de chaine pour ne pas causer de problèmes
      mot_abr = malloc((strlen(mot_tmp)+1)*sizeof(char)); // Alloue la mémoire +1 pour le \0
      strcpy(mot_abr,mot_tmp);	// Copie la chaine de caractère dans l'autre
      memset(mot_tmp, 0, 64*sizeof(char));	// réinitialise la mémoire pour pouvoir prendre un autre mot
      A = insert_mot(A, mot_abr, ligne_actuelle);	// On insére le mot copié
    }
    else if(isalpha(T[i])) {	// Si c'est un therme alphabétique seulement, on met en minuscule et on concatène
      T[i] = tolower(T[i]);
      strncat(mot_tmp, &T[i], 1);
    }
    // Si il y a un retour à la ligne on passe à la ligne suivante
    if(T[i] == '\n')
      ligne_actuelle++;
  }

  printf("Construction AVL : %f s\n", chrono_lap()); 
  // On libère les valeurs allouées 
  free(T);
  free(mot_tmp);
  free(mot_abr);
  
  return A;
}
```



### *AVL* *search (AVL* A, char* mot_a_chercher)

La fonction AVL_search permet de chercher un mot dans un arbre A donné. On va ici ce servir de la fonction `strcmp` vu plus haut. C'est une fonction récursive qui va chercher dans chaque nœuds le mot. Comme c'est un ABR, il suffit de savoir quel mot est plus grand (dans le sens lexicographique toujours) et d'aller dans le sous arbre gache ou droite en fonction du résultat et de recommencer cette opération sur le fils où l'on se situe.  

```c
AVL *search (AVL* A, char* mot_a_chercher) {
  AVL* result = A; // AVL courant qui stocke le résultat

  if (result && strcmp(result->mot, mot_a_chercher)) {
    if(strcmp(result->mot, mot_a_chercher) < 0)
      result = search(result->fd, mot_a_chercher);
    else
      result = search(result->fg, mot_a_chercher);
  }

  if (!A) printf("mot non trouvé\n");

  return result;
}
```

## Autres fonctions utiles

### *void* libere_memoire(AVL *A)

Fonction permettant de libérer la mémoire de façon récursive. On libère la mémoire du mot et du tableau de ligne pour ensuite free l'arbre entier. 

```c
void libere_memoire(AVL *A){
  if(A){
    libere_memoire(A->fg);
    libere_memoire(A->fd);
    free(A->mot);
    free(A->ligne);
    free(A);
  }
}
```

### *char* *mot_alea (AVL *A)

Il est demander dans le sujet de faire une recherche aléatoire si aucun mot n'est en argument du main. On créer alors une fonction qui sera répété n fois dans notre main. Elle a pour but de prendre le mot d'un nœud au hasard dans notre arbre pris en argument de la fonction. Certaines valeurs sont prise arbitrairement pour pouvoir un "manipuler" le hasard.

```c
char *mot_alea (AVL *A) {
  int H = AVL_height(A);
  int val_alea = rand()%H+1;

  if (val_alea == 1)                // Valeur pris arbitrairement
    return A->mot;

  if (A->fg && (val_alea<(H/2)))    // Arbitrairement, si la valeur aleatoire est
    return mot_alea(A->fg);         // plus plus petite que la hauteur du noeud
  if (A->fd && (val_alea > (H/2)))  // aux feuilles, on va chercher dans le
    return mot_alea(A->fd);         // fils gauche, dans le fils droit sinon.

  return A->mot;
}
```

### *void* affichage_avl (AVL* A, char* text)

Il est demander dans le sujet d'afficher les lignes qui correspondent aux mots mis en argument de la fonction main. Pour ça on va utiliser la fonctin `first_char` qui va nous permettre de print la ligne du caractère `L[A->ligne[x]]` au caractère `L[A->ligne[x+1]]` et d'afficher ces caractère avec le tableau de caractères obtenue avec `char_to_tab`.

```c
void affichage_avl (AVL* A, char* text) {
  char* T = char_to_tab (text);
  int* L = first_char (text);

  printf("\nOccurence du mot %s : %d\n\n",A->mot,A->occ);

  for (int i=0 ; i<A->occ ; i++) {
    if (A->ligne[i] != A->ligne[i+1]){
      printf("Ligne %d : ",A->ligne[i]);
      for (int k=L[A->ligne[i]] ; k<L[A->ligne[i]+1] ; k++)
        printf("%c",T[k]);
    }
  }

  free(T); free(L);
}
```

## *int* main (char* argv, char* argc[ ] )

La fonction principale réunit toute les fonctions principales faites. Elle a pour but 

* De prendre le fichier texte pris en argument
* D'afficher l’occurrence et les lignes du mot pris en argument avec le temps mis
* Si il n'y a pas d'argument on prend 1000 mots au hasard et on affiche le temps que ça a pris en détails

Pour ça on créer un AVL qui 'noeud_a_trouver' qui correspond au nœud du mot à trouver. Cette variable nous servira aussi bien pour les 1000 mots que les mots pris en argument dans le main. On a toujours `chrono_reset()` qui lance le chrono et `chrono_lap()` qui l'arrête. On utilise  `srand(getpid())` pour avoir une seed qui fait changer notre rand à chaque fois.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chrono.h"
#include "text_to_tab.h"
#include "avl_fct.h"
#include "avl_search.h"

int main (int argc, char *argv[]){
  AVL *A = NULL;
  AVL *noeud_a_trouver = NULL;

  A = tab_to_AVL(A,argv[1]);

  // Si pas d'arguments
  if(!argv[2]) {
    char* mot;
    srand(getpid());
    chrono_reset();
    for (int i=0 ; i<1000 ; i++) {
      mot = mot_alea(A);
      noeud_a_trouver = search(A,mot);
    }

    printf("Recherche pour 1000 mots : %f s\n\n", chrono_lap());
  }

  // S'il y a des arguments
  for(int k=2 ; k<argc ; k++) {  // On commence par 2 pour obtenir le/les mots pris en, argument
    chrono_reset();
    noeud_a_trouver = search(A,argv[k]);
    affichage_avl(noeud_a_trouver,argv[1]);
    printf("\nTemps de recherche du mot %s : %f s\n\n",argv[k], chrono_lap());
  }

  libere_memoire(A);
  return 0;
}
```

## Références

https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/arbres%20AVL.htm
https://fr.wikipedia.org/wiki/Arbre_AVL
https://fr.wikipedia.org/wiki/Rotation_d%27un_arbre_binaire_de_recherche