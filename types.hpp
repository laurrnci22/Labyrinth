/* ----------------------------------------------------------------- 
 * définition des structures nécessaire pour représenter la matrice
 * d'adjacence du graphe de parcours d'un labyrinthe
 * -----------------------------------------------------------------
 */

/*
 * structure d'un maillon de la liste chaînée associé 
 * à chaque ligne de la matrice
 */
struct Maillon {
  int l, c; // coordonnées du noeud dans la grille du labyrinthe
  Maillon *suiv;// élément suivant  sur la ligne
};

/*
 * structure représentant une matrice d'adjacence sous forme
 * de matrice creuse. La structure contient un tableau de lignes, chaque
 * ligne étant représentée par une liste chaînée de "maillons".
 */
struct MatriceAdjacence {
  int ordre; // nombre de sommets du graphe
  Maillon* *lignes; // tableau à allouer de taille "ordre", représentant les lignes de la matrice
};

/* ----------------------------------------------------------------- 
 * définition de la structure de labyrinthe
 * -----------------------------------------------------------------
 */
struct labyrinthe {
  int largeur, hauteur;// nombre de colonnes et de lignes du labyrinthe
  int *cell; // cellules du labyrinthes
  bool *mursV;// état des murs verticaux des cellules
  bool *mursH;// état des murs horizontaux des cellules
};


/* ----------------------------------------------------------------- 
 * définition de la structure d'une coordonnée et d'un chemin
 * -----------------------------------------------------------------
 */
struct coordonnee {
  int x, y; // abscisse et ordonnée d'une case du labyrinthe
};


struct chemin {
  int lg; // longueur du chemin (nb de cases du tableau etape)
  coordonnee *etape;// les coordonnées des différents sommets à parcourir
};


/* ----------------------------------------------------------------- 
 * définition des structures  nécessaire pour représenter une liste
 * doublement chaînée de nombre entiers
 * -----------------------------------------------------------------
 */

/*
 * structure d'un maillon de liste chaînée d'entiers 
 */
struct MaillonEntier {
  int valeur; // valeur stockée dans le maillon
  MaillonEntier *suiv;// élément suivant dans la file
  MaillonEntier *prec;// élément précédent dans la file
} ;

/*
 * structure d'une file de type FIFO pouvant contenir des entiers
 */
struct Fifo {
  MaillonEntier *in;
  MaillonEntier *out;
};


/*
 * type énuméré permettant de représenter les couleurs
 */
enum Couleur {BLANC, GRIS, NOIR};

// valeur indéfinie pour un indice de parent
#define INDEFINI  -1

// valeur infinie pour une distance enre sommets
#define INFINI  -1
