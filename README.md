# Projet Langage C

> Projet d'IN301 - Franck Quessette
>
> Projet fait par Ines Lebib et Nicolas Fond-Massany

## Introduction

Ce README sert à expliquer le plus précisément possible le code du projet. Tous les codes sont dans le répertoire github. Le code est ici repris partie par partie avec des annotations ainsi qu'une description. Ce projet est sur Github pour simplifier les échanges de codes entre binôme et pour mieux prendre en main cet outil.

## Canevas

Pour commencer, on doit instaurer un cadre qui nous permettra de créer les éléments utiles aux projet. On créer donc un Makefile, principal.c, text_to_tab.c, text_tot_tab.h, avl_fct.c, avl_fct.h, avl_search.c et avl_search.h. Les fichiers permettant de chronométré notre programme sont donnés avec le sujet du projet.  On peut faire, une fois le terminal dans le répertoire voulue :

` touch Makefile principal.c text_to_tab.c text_to_tab.h avl_fct.c avl_fct.h avl_search.c avl_search.h`

## Du .txt au tableau

Le but de cette partie est de mettre des caractères dans un tableau de caractères et de savoir quelle est le premier caractère de chaque lignes.
On va créer 2 tableaux: ```T[ ]``` de type *char* qui contiendra tous les caractères du fichier et ```L[ ]``` de type *int* qui contiendra la position du premier caractère de chaque lignes de façon à savoir que, par exemple, c'est le caractère 243 qui commence la ligne 4. On notera alors `L[4] = 243`.

Pour simplifier la lecture du code et mieux le comprendre, on découpera correctement le code en fonctions.

On va alors créer **2 fonctions principales**:

* `char *first_char(char *text)` qui va nous permettre d'associer les lignes et le premier caractère qu'elle contient. Elle renverra la variable *int* L décrit plus haut.
* `char *char_to_tab(char *text)` qui va nous permettre de mettre dans un tableau de caractère tout le texte. Elle renverra la variable *char* T décrit plus haut.

Ces 2 fonctions ont une étoile ***** car elle renvoie un pointeur qui va nous permettre de reprendre le résultat alloué des fonctions pour les réutiliser, notamment dans notre fichier principal.c .

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

Après avoir ouvert notre fichier en mode *read* et vérifier que notre fichier était bien présent dans le dossier où le programme se trouve, on va chercher à mettre dans notre variable *L* les premier caractère de chaque ligne. Cela revient à prendre en compte les caractères juste après un retour à la ligne. Cela se traduit en langage C par `if(fgetc(F) == '\n')`. Mais avant cela on doit allouer notre mémoire pour créer le tableau *L*. On fait pour ça appel à la fonction `malloc` de la librairie *stdlib*. Comme on veux avoir un tableau d'une taille égale aux nombres de lignes du texte, on va faire `int *L = malloc(nbr_lignes*(sizeof(int)));`. Là encore, on a besoin d'une fonction qui nous permettrait de connaître le nombre de ligne du texte. On va alors créer la fonction `int cmt_lignes (char *text)` qui va compter (*cmt*) les lignes du texte.

***

#### *int* cmt_lignes (char *text)

Pour compter les lignes, on a besoin de lire notre fichier et de simplement compter le nombre de `\n` qui correspond au retour chariot.

On ouvre donc notre fichier : `FILE *F = fopen(text,"r");` puis on vérifie qu'il soit bien ouvert : `verif_fic(text);`  . On cherche à faire une boucle qui compte le nombre de `\n` dans ce fichier. Pour faire une boucle, il faut savoir où s’arrêter, c'est pourquoi il y a principalement 2 manières de faire.

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

Revenons à notre code dans sa globalité. On a maintenant, le nombre de caractères du texte, le nombre de lignes et l'allocation mémoire de *L (alloué autant de fois qu'il y a de lignes). On va maintenant passer à la boucle qui va permettre de faire correspondre chaque caractères de début de ligne à sa ligne.

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

  int ligne = 1;

  for (int i=0 ; i<=nbr_char ; i++) {	// Parcours du texte
    if(fgetc(F) == '\n') {				// Si il y a un retour à la ligne
      L[ligne] = i+1;						// Le caractère suivant = 1er char de la ligne
      ligne++;								// On avance d'un ligne
    }
  }

  fclose(F);
  return L;
}
```

### *char*  char_to_tab (char *text)

Cette fonction sera beaucoup moins longue que la précédente puisqu'elle utilisera les fonctions vu précédemment.

Premièrement on ouvre le fichier puis on vérifie s'il est ouvert. Pour ça toujours la méthode et la même fonction `verif_fic`. Ensuite, on va chercher à mettre chaque caractères dans le tableau. Pour ça on va parcourir le fichier avec une boucle encore une fois en utilisant *fgetc*. Cette fonction trouve en effet très bien sa place ici puisqu'elle va nous permettre de mettre dans *T[ i ]* tous les caractères.

Pour ça on va avoir besoin d'une variable nbr_char qui aura pour fonction à la fois de balayer le texte avec une boucle mais aussi d'allouer la bonne capacité de mémoire pour notre variable T. On met alors dans une variable *nbr_char* la taille du fichier avec notre fonction `taille_fic`. On alloue ensuite la mémoire de T en faisant `char *T = malloc(nbr_char*(sizeof(char)));` . On vérifie si la mémoire est bien alloué. On fait un for qui va permettre de parcourir le mot et d'implémenter chaque caractère du texte dans notre tableau.

Finalement on ferme le fichier puis on retourne T.

```c
char *char_to_tab (char *text) {
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

***

## Fonctions sur les AVL pour des chaînes de caractères

Il a plusieurs manières de créer un AVL sur des *string* (chaîne de caractères). Ici, on ne traitera qu'une seule méthode assez simple à comprendre si on connaît les principes d'un AVL. Que ce soit des *int*, des *float* ou des char, la méthode reste quasiment la même. C'est principalement les structures qui différent. **Cette partie concerne les fonctions permettant de créer cet AVL mais pas de le créer directement, nous verrons ça dans la partie suivant.**
On va partir de la base. On veux trouver un mot qui se situe dans le texte avec la commande `/dico text mot_à_trouver`. Pour que ce soit efficace, on va construire un arbre binaire de recherche (ABR). La recherche sera alors bien plus rapide car on effectuera moins d'itérations que si on avait à prendre chaque mot du texte. Pour aller plus vite encore, on va construire un AVL. Un ABR équilibré sera bien plus efficace qu'un AVL, on effectuera $log(n)$ itération ce qui est bien plus intéressant.

On va partitionner le code en plusieurs fonctions et plusieurs fichiers. Un fichier qui contient tout ce qu'il faut pour manipuler un AVL et un autre pour chercher dans cet AVL le mot voulue. On mettra enfin tout ça dans le `principal.c` pour que le programme puisse comprendre la commande entrée avec le ou les mot(s) à chercher et le texte dans lequel il faut chercher.

Les fichiers seront nommés `avl_search.c`, `avl_search.h,` `avl_fct.c` et  `avl_fct.h`.

 ### avl_fct.c / avl_fct.h

Toutes les fonctions qui vont nous être utiles dans avl_search sont ici crée. Il n'y aura pas de fonctions de recherche ni d'insertion du tableau T fait précédemment. Pour savoir quelle fonctions on doit créer, on va regarder la définitions d'un AVL. Le principe fondamental d'un AVL est qu'il est équilibré. On part du principe qu'il peux maximum admettre 1 de différence de hauteur dans le sous arbres droit ou gauche de la racine. Donc un AVL est un ABR H-équilibré avec un delta de 1 de hauteur maximum. Il nous faudra alors:

* Les fonctions classiques de création d'un ABR : initialisation d'un nœud, insertion d'un nœud.
* Une fonction qui trouve la hauteur d'un arbre afin de connaître le déséquilibre d'un arbre avec une autre fonction qui indiquera ce déséquilibre.
* S'il y a un déséquilibre c'est qu'il faut rééquilibré l'arbre. Il nous faudra donc des fonctions qui font des rotations droite gauche, gauche droite, droite droite et gauche gauche.
* Si on a les fonctions de rotation il faut une autre fonction capable de déterminer quelle rotation il faut faire. On créera donc une fonction permettant d'équilibrer l'arbre.

Avec ces fonctions, on aura tous les outils en main pour chercher un mot dans notre texte dans le programme `avl_search.c`.

On créer premièrement une structure nous permettant de manipuler simplement l'arbre. On implémente cette structure dans notre `avl_fct.h`.

```c
typedef struct avl_mots {
  int ligne, occ; // Ligne, Occurence
  char* mot;
  struct avl_mots *fg, *fd; // fils droit , fils gauche
} AVL;
```

#### *AVL* creer_avl ()

Cette fonction permettra de créer un AVL, d'initialiser un nœud en même temps puisqu'on créer un nœud qui sera peut-être une racine si elle possède par la suite un fils droit et/ou gauche. Cette fonction aura aussi pour but d'allouer correctement la mémoire et d'indiquer s'il y a une erreur dans l'allocation mémoire. Comme on initialise les valeurs de notre futurs AVL, on mettra dans chacune d'entre elles des valeurs NULL. Il faut aussi allouer la mémoire pour le mot car c'est une chaîne de caractère qui a besoin de mémoire pour être réutilisé après. On initialise l'occurence à 1 puisque si on créer un mot, c'est qu'il existe au moins 1 fois dans le texte. **Il faudra par la suite libérer la mémoire à la fois de l'arbre et du mot.**

```c
AVL *cree_noeud (char* mot) {
  AVL* A = NULL;						// Pointeurs vers NULL
  A = malloc(sizeof(AVL));				// Allocation du pointeurs
  if (A == NULL) allocation_erreur();	// Vérification de l'allocation

  A->fg = NULL; A->fd = NULL;			// les fils sont à NULL
  A->occ = 1;							// L'occurence est a 1

  A->mot = malloc(strlen(mot)*sizeof(char*));	// Allocation mémoire du mot
  if (A->mot == NULL) allocation_erreur();

  A->mot = mot;

  return A;
}
```

 #### *int* height_arbre (AVL A)

Comme dit précédemment cette fonction nous permettra de connaître le déséquilibre d'un arbre dans une autre fonction. La hauteur d'un arbre correspond au nombre maximum de nœuds entre la racine de l'arbre entier et une feuille. Il faudra donc comparer le fils droit et gauche de chaque racines afin de savoir lequel est la plus grande valeur. Pour ça on va faire une fonction récursive qui va retourner la plus grande valeur entre le fils droit et le fils gauche. A ce moment là on comparera le fils droit et gauche de chaque sous arbres.
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

Cette fonction a pour but de calculer le déséquilibre d'un arbre. Comme on a fait précédemment une fonction qui calcul la hauteur d'un arbre, il ne nous reste plus qu'à ajouter à une variable *int* la hauteur du fils droit et de soustraire cette valeur à la hauteur du fils gauche. On agit sur les variables seulement si elle existe, on va donc mettre un `if (/*variable*/)` .

```c
int calcul_desequilibre(AVL *A) {
  int difference = 0;   				 	// Variable qui stock le déséquilibre

  if(A->fd) difference = AVL_height(A->fd);		// la difference vaux la hauteur du sous arbre droit  
  if(A->fg) difference -= AVL_height(A->fg);	// On fait la différence avec la hauteur du sous arbre gauche

  return difference;
}
```

#### Les rotations

Les rotations vont nous permettre d'équilibrer l'arbre. Il existe 4 rotations : gauche, droite, droite gauche et gauche droite.

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
On détail ici à chaque variables ce que fait l’affectation :

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

Rotation droite gauche

![img](https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/Arbres%20AVL_fichiers/image010.jpg)

Rotation gauche droite

![img](https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/Arbres%20AVL_fichiers/image012.jpg)

> Figures du site https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/arbres%20AVL.htm

#### *AVL* *equilibre (AVL *A)

Cette fonction permet d'équilibré un ABR pour qu'il devienne un AVL. Le but est de parcourir tout l'arbre et de détecter si le déséquilibre vaux 2 ou -2. On sait que :

* Si le nœud a une différence de profondeur de sous-arbre de 2 :
  * Si le fils gauche à une différence de -1 il faut faire une rotation droit gauche
  * Si le fils gauche à une différence de 1 il faut faire une rotation gauche simple
* Si le nœud a une différence de profondeur de sous-arbre de -2 :
  * Si le fils droit à une différence de -1 il faut faire une rotation droite simple
  * Si le fils gauche à une différence de 1 il faut faire une rotation gauche droite

On peut représenter ça sous forme de tableau :

| Différence                  | Équilibre de la racine : 2 | Équilibre de la racine : -2 |
| --------------------------- | -------------------------- | --------------------------- |
| **Équilibre fils gauche 1** | Simple Gauche              | Gauche Droite               |
| **Équilibre fils droit -1** | Droite Gauche              | Simple Droite               |

La fonction est récursive afin d'avoir un code plus simple.

```c
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
```

## Recherche dans un AVL
### *AVL* *insert_mot (AVL *A, char *mot)
Cette fonction permet d'insérer un mot dans un arbre vide ou non. Cette fonction nous permettra pas la suite de mettre tous les mots du tableau contenant les caractères (on aura juste à dire que s'il y a un espace alors la variable qui vient de stocké les caractères précédent est un mot). Cette fonction prend en compte les arbres vides donc pas de prérequis sur l'allocation, la fonction fait tout.
La fonction est assez simple puisque c'est un ABR de string avec l'ajout d'une fonction qui équilibre (et devienne un AVL).
Pour comparer les chaînes de caractères du mot et du mot de la racine on va utiliser la bibliothèque `<string.h>` pour son `strcmp(mot A, mot B)`. Cette fonction renvoie 0 si les 2 mots sont les même (sachant qu'un mot ne doit pas apparaître 2 fois dans l'arbre), si le mot A est plus petit que le mot B une valeur négative sera retournée, si le mot A est plus grand que le mot B une valeur plus grande est retournée. Tout ça au sens lexicale.
Une fois qu'on sait si le mot doit aller à gauche ou à droite de la racine, on vérifie que l'endroit où doit aller le mot n'est pas NULL car si c'est le cas il suffit de mettre le mot dans ce fils puis de retourner la valeur. Sinon la fonction s’appelle elle même. En faisant ça, elle parcourt l'arbre jusqu'à trouver un fils NULL où il peux aller.

```c
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
```

### AVL *tab_to_AVL (AVL* A, char *fic)

```c
AVL *tab_to_AVL (AVL* A, char *fic) {
  char *T = char_to_tab(fic);				// Tableau qui prend tous les caractères du texte
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
```



### AVL *search (AVL* A, char* mot_a_chercher)



## Références

https://cours.etsmtl.ca/SEG/FHenri/inf145/Suppl%C3%A9ments/arbres%20AVL.htm
https://fr.wikipedia.org/wiki/Arbre_AVL
https://fr.wikipedia.org/wiki/Rotation_d%27un_arbre_binaire_de_recherche
