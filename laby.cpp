#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>

using namespace std;

#include "types.hpp"

#define CELLSIZE 50 // taille d'un côté d'une case du labyrinthe
#define MARGE 10    // marge du dessin
#define TEXTSIZE 10 // taille d'affichage du texte

// --------------------------------------------------------------------
// prédéfinition des fonctions liées à la gestion d'un labyrinthe
// --------------------------------------------------------------------
void initialiserLabyrinthe(labyrinthe &laby, int largeur, int hauteur);
void effacerLabyrinthe(labyrinthe &laby);
void genererLabyrinthe(labyrinthe &laby);
void dessinerLabyrinthe(labyrinthe &laby, const string &nomFichier);
void testAffichage(labyrinthe laby);

void creerMatrice(MatriceAdjacence &m, int taille);
void effacerMatrice(MatriceAdjacence &mat);
void afficherMatrice(MatriceAdjacence mat);
void remplirMatrice(MatriceAdjacence &mat, const labyrinthe &lab);
void dessinerMatrice(const MatriceAdjacence &mat, const labyrinthe &laby,
                     const string &nom);

void saisirCoordonnees(coordonnee &deb, coordonnee &fin, int largeur, int hauteur);

void initialiserFifo(Fifo &file);
bool estVide(Fifo file);
void ajouter(Fifo &file, int v);
 void afficheListe(Fifo file);//pour le test de la liste doublement chainÃ©e
 int retirer(Fifo &file);
 void parcoursEnLargeur(MatriceAdjacence mat, int sommetDepart,int sommetArrive,Couleur *coul, int *dist, int *parent,int largeur );
void afficheCouleurs(MatriceAdjacence mat,Couleur *coul);
void afficheDistances(MatriceAdjacence mat,int *dist);
void afficheParents(MatriceAdjacence mat,int *parents);
void remplirTabChemin(int sf, int *parent,coordonnee  *tab , int taille,int largeur);
chemin calculerChemin(const MatriceAdjacence &mat, coordonnee deb, coordonnee fin, int largeur);
void dessinerSolution(const labyrinthe &laby, const chemin &ch, const string &nomFichier);
// --------------------------------------------------------------------
// prédéfinition des fonctions liées à l'écriture dans les fichiers SVG
// --------------------------------------------------------------------

// création du fichier et ouverture du flot d'écriture associé
// retourne false si le fichier n'a pas pu être ouvert.
bool ouvrirFichierSVG(const string &nomFichier, // nom du fichier à créer
                      ofstream &out, // flot d'écriture ouvert par la fonction
                      int largeur,   // largeur de l'image en pixels
                      int hauteur);  // hauteur de l'image en pixels

// fermeture du flot d'écriture et du fichier associé
void fermerFichierSVG(
    ofstream &out); // flot d'écriture à fermer par la fonction

// écriture d'une ligne dans le flot de sortie
void ligne(ofstream &out,       // flot d'écriture dans lequel ajouter la ligne
           int x1, int y1,      // coordonnées du point de départ de la ligne
           int x2, int y2,      // coordonnées du point d'arrivée de la ligne
           const string &color, // couleur de tracé de la ligne
           int width);          // épaisseur de tracé de la ligne

// écriture d'un rectangle dans le flot de sortie
void rect(ofstream &out, // flot d'écriture dans lequel ajouter le rectangle
          int x, int y,  // coordonnées du point supérieur gauche du rectangle
          int width,     // largeur du rectangle en pixels
          int height,    // hauteur du rectangle en pixels
          const string &color); // couleur de tracé du rectangle

// écriture d'un texte dans le flot de sortie
void text(ofstream &out,        // flot d'écriture dans lequel ajouter le texte
          int x, int y,         // coordonnées du point auquel placer le texte
                                // le texte est centré par rapport à ce point
          int size,             // hauteur des caractères du texte en pixels
          const string &txt,    // le texte à ajouter
          const string &color); // la couleur de tracé du texte

// --------------------------------------------------------------------
// prédéfinition de la fonction de conversion d'une valeur entière
// en chaîne de caractères
// --------------------------------------------------------------------
string intToString(int v);

// --------------------------------------------------------------------
// programme principal
// --------------------------------------------------------------------

int main(int argc, char *argv[]) {
  srand(time(NULL));
  labyrinthe laby;
  int largeur, hauteur;
  string nomFichier;
  MatriceAdjacence mat = {0, nullptr};

  if (argc == 1) {
    largeur = 10;
    hauteur = 10;
    nomFichier = "solution.svg";
  } else if (argc == 4) {
    largeur = atoi(argv[1]);
    hauteur = atoi(argv[2]);
    nomFichier = argv[3];
  } else{
    cout << "le syntaxe est :" << argv[0] << " <largeur> <hauteur> <nomFichier.svg>" << endl;
    return 0;
  }

  initialiserLabyrinthe(laby, largeur, hauteur);
  genererLabyrinthe(laby);
  dessinerLabyrinthe(laby, "laby.svg");
  int taille = largeur * hauteur;
  creerMatrice(mat, taille);
  remplirMatrice(mat, laby);
  afficherMatrice(mat);

 dessinerMatrice(mat, laby, "graphe.svg");

  coordonnee deb,fin;
  // deb.x = 19;
  // deb.y = 15;

cout <<"deb x ?";
cin>>deb.x;
cout <<"deb y ?";
cin>>deb.y;
  
cout <<"fin x ?";
cin>>fin.x;
cout <<"fin y ?";
cin>>fin.y;

saisirCoordonnees(deb,fin, largeur, hauteur);
  chemin chemins;
chemins = calculerChemin(mat,deb, fin,largeur);
    for(int i = 0 ; i < chemins.lg ; i++)
     cout<<"("<<  chemins.etape[i].x <<","<< chemins.etape[i].y<<")";

  dessinerSolution(laby,chemins,nomFichier);

  return 1;
}

// --------------------------------------------------------------------
// définition des fonctions liées à la gestion d'un labyrinthe
// --------------------------------------------------------------------

// à compléter ...

void initialiserLabyrinthe(labyrinthe &laby, int largeur, int hauteur) {

  laby.largeur = largeur;
  laby.hauteur = hauteur;
  laby.cell = new int[largeur * hauteur]; // cellules du labyrinthes
  laby.mursV = new bool[(laby.largeur + 1) * laby.hauteur];
  laby.mursH = new bool[laby.largeur * (laby.hauteur + 1)];

  for (int i = 0; i < laby.largeur * laby.hauteur; i++)
    laby.cell[i] = i;
  for (int i = 0; i < (laby.largeur + 1) * laby.hauteur; i++)
    laby.mursV[i] = true;
  for (int i = 0; i < laby.largeur * (laby.hauteur + 1); i++)
    laby.mursH[i] = true;
}

void testAffichage(
    labyrinthe laby) { // Pour tester le fonctionnement des tableaux
  cout << "Cellule !" << endl;
  for (int i = 0; i < laby.hauteur; i++) {
    for (int j = 0; j < laby.largeur; j++) {
      cout << laby.cell[i * laby.largeur + j] << "  ";
    }
    cout << endl;
  }
  cout << endl;

  cout << "Murs Vertical !" << endl;
  for (int i = 0; i < laby.hauteur; i++) {
    for (int j = 0; j < laby.largeur + 1; j++) {
      if (laby.mursV[i * (laby.largeur + 1) + j] == true)
        cout << " T ";
      else
        cout << " F ";
    }
    cout << endl;
  }
  cout << endl;

  cout << "Murs horizontal !" << endl;
  for (int i = 0; i < laby.hauteur + 1; i++) {
    for (int j = 0; j < laby.largeur; j++) {
      if (laby.mursH[i * laby.largeur + j] == true)
        cout << " T ";
      else
        cout << " F ";
    }
    cout << endl;
  }
}

void effacerLabyrinthe(labyrinthe &laby) {
  laby.largeur = 0;
  laby.hauteur = 0;
  delete[] laby.mursH;
  delete[] laby.mursV;
  delete[] laby.cell;
  laby.mursH = nullptr;
  laby.mursV = nullptr;
  laby.cell = nullptr;
}

void dessinerLabyrinthe(labyrinthe &laby, const string &nomFichier) {
  ofstream out;

  ouvrirFichierSVG(nomFichier, out, laby.largeur * CELLSIZE,
                   laby.hauteur * CELLSIZE);
  rect(out, 0, 0, laby.largeur * CELLSIZE, laby.hauteur * CELLSIZE, "white");

  for (int i = 0; i < laby.hauteur; i++) {
    for (int j = 0; j < laby.largeur + 1; j++)
      if (laby.mursV[i * (laby.largeur + 1) + j] == true)
        ligne(out, j * CELLSIZE, i * CELLSIZE, j * CELLSIZE, (i + 1) * CELLSIZE,
              "black", 1);
  }

  for (int i = 0; i < laby.hauteur + 1; i++) {
    for (int j = 0; j < laby.largeur; j++)
      if (laby.mursH[i * laby.largeur + j] == true)
        ligne(out, j * CELLSIZE, i * CELLSIZE, (j + 1) * CELLSIZE, i * CELLSIZE,
              "black", 1);
  }
  
  //pour mettre des chiffres à l'interieur du labyrinthe 
  // for (int i = 0; i < laby.hauteur; i++) {
  //   for (int j = 0; j < laby.largeur; j++) {
  //     text(out, (CELLSIZE * j) + (CELLSIZE * 1 / 2),
  //          (CELLSIZE * i) + (CELLSIZE * 1 / 2), TEXTSIZE,
  //          intToString(laby.cell[i * laby.largeur + j]), "red");
  //   }
  //   cout << endl;
  // }

  fermerFichierSVG(out);
}

void genererLabyrinthe(labyrinthe &laby) {

  int type_mur, xmur, ymur, mur, lig, col, voisin, pos;
  int cpt = (laby.hauteur * laby.largeur) - 1;

  while (cpt != 0) {
    lig = rand() % (laby.hauteur);
    col = rand() % (laby.largeur);
    type_mur = rand() % 2;
    pos = laby.cell[lig * laby.largeur + col];

    if (type_mur == 0) { // mur vertical
      ymur = col + rand() % 2;
      while (ymur == 0 ||
             ymur == laby.largeur) { // pour ne pas enlever les murs du bord
        ymur = col + rand() % 2;
      }
      if (ymur == col) { // Mur a gauche
        if (laby.cell[lig * laby.largeur + (col - 1)] !=
            laby.cell[lig * laby.largeur + col]) {
          laby.mursV[lig * (laby.largeur + 1) + ymur] = false;
          voisin = laby.cell[lig * laby.largeur + (col - 1)];
          cpt = cpt - 1;
        }
      }
      if (ymur == col + 1) { // Mur a droite
        if (laby.cell[lig * laby.largeur + (col + 1)] !=
            laby.cell[lig * laby.largeur + col]) {
          laby.mursV[lig * (laby.largeur + 1) + ymur] = false;
          voisin = laby.cell[lig * laby.largeur + (col + 1)];
          cpt = cpt - 1;
        }
      }
    }

    else { // mur horizontal
      xmur = lig + rand() % 2;
      while (xmur == 0 ||
             xmur == laby.hauteur) { // pour ne pas enlever les murs du bord
        xmur = lig + rand() % 2;
      }
      if (xmur == lig) { // Mur en haut
        if (laby.cell[(lig - 1) * laby.largeur + col] !=
            laby.cell[lig * laby.largeur + col]) {
          laby.mursH[xmur * (laby.largeur) + col] = false;
          voisin = laby.cell[(lig - 1) * laby.largeur + col];
          cpt = cpt - 1;
        }
      }
      if (xmur == lig + 1) { // Mur en bas
        if (laby.cell[(lig + 1) * laby.largeur + col] !=
            laby.cell[lig * laby.largeur + col]) {
          laby.mursH[xmur * (laby.largeur) + col] = false;
          voisin = laby.cell[(lig + 1) * laby.largeur + col];
          cpt = cpt - 1;
        }
      }
    }
    for (int i = 0; i < laby.hauteur; i++) {
      for (int j = 0; j < laby.largeur; j++) {
        if (laby.cell[i * laby.largeur + j] == voisin)
          laby.cell[i * laby.largeur + j] =
              pos; // pour mettre a jour tous les elements
      }
    }
  }
}

void creerMatrice(MatriceAdjacence &m, int taille) {
  if (m.lignes != nullptr)
    delete m.lignes;
  // initialisation du nombre de lignes/colonnes de la matrice
  m.ordre = taille; // hauteur * largeur
  // allocation mémoire du tableau de lignes
  m.lignes = new Maillon *[taille];
  // initialisation de chaque ligne à "vide"
  for (int i = 0; i < taille; i++)
    m.lignes[i] = nullptr;
}

void effacerMatrice(MatriceAdjacence &mat) {
  for (int l = 0; l < mat.ordre; l++) { // effacer chaque ligne
    while (mat.lignes[l] != nullptr) {  // tq la ligne n'est pas vide
      // effacer le premier élément qui s'y trouve
      Maillon *cour = mat.lignes[l]; // 1er élément de la liste
      mat.lignes[l] = cour->suiv;    // élément suivant éventuel
      delete cour;                   // effacer le 1er élement courant
    }
  }
  // effacer le tableau de lignes
  delete mat.lignes;
  mat.lignes = nullptr;
  // raz de la taille
  mat.ordre = 0;
}

void afficherMatrice(MatriceAdjacence mat) {
  if (mat.lignes != NULL) {
    for (int i = 0; i < mat.ordre; i++) {
      Maillon *tmp = mat.lignes[i];
      if (tmp != nullptr) {
        cout << "noeud " << i << " (" << tmp->c << "," << tmp->l << ")   : ";
        tmp = tmp->suiv;
        while (tmp != NULL) {
          cout << "(" << tmp->c << "," << tmp->l << ") ";
          tmp = tmp->suiv;
        }
        cout << endl;
      } else
        cout << " Liste vide !\n";
    }
  }
}

void remplirMatrice(MatriceAdjacence &mat, const labyrinthe &lab) {
  int cpt_ligne = 0;
  Maillon *tmp;

  for (int i = 0; i < lab.hauteur; i++) {
    for (int j = 0; j < lab.largeur; j++) {
      // pour mettre les coordonnes de chaque ligne !!
      mat.lignes[cpt_ligne] = new Maillon; // sur la ligne
      mat.lignes[cpt_ligne]->l = i;
      mat.lignes[cpt_ligne]->c = j;
      mat.lignes[cpt_ligne]->suiv = nullptr;

      tmp = mat.lignes[cpt_ligne];

      if (lab.mursV[i * (lab.largeur + 1) + j] == false) { // a gauche !
        Maillon *nouveau = new Maillon;
        nouveau->suiv = nullptr;
        nouveau->c = j - 1;
        nouveau->l = i;
        tmp->suiv = nouveau;
        tmp = tmp->suiv;
      }
      if (lab.mursV[i * (lab.largeur + 1) + j + 1] == false) { // a droite !
        Maillon *nouveau = new Maillon;
        nouveau->suiv = nullptr;
        nouveau->c = j + 1;
        nouveau->l = i;
        tmp->suiv = nouveau;
        tmp = tmp->suiv;
      }
      if (lab.mursH[i * (lab.largeur) + j] == false) { // en haut
        Maillon *nouveau = new Maillon;
        nouveau->suiv = nullptr;
        nouveau->c = j;
        nouveau->l = i - 1;
        tmp->suiv = nouveau;
        tmp = tmp->suiv;
      }
      if (lab.mursH[(i + 1) * (lab.largeur) + j] == false) { // en bas
        Maillon *nouveau = new Maillon;
        nouveau->suiv = nullptr;
        nouveau->c = j;
        nouveau->l = i + 1;
        tmp->suiv = nouveau;
        tmp = tmp->suiv;
      }
      cpt_ligne++;
    }
  }
}

void dessinerMatrice(const MatriceAdjacence &mat, const labyrinthe &laby,
                     const string &nom) {
  ofstream out;

  ouvrirFichierSVG(nom, out, laby.largeur * CELLSIZE, laby.hauteur * CELLSIZE);
  rect(out, 0, 0, laby.largeur * CELLSIZE, laby.hauteur * CELLSIZE, "white");

  for (int i = 0; i < laby.hauteur; i++) {
    for (int j = 0; j < laby.largeur + 1; j++)
      if (laby.mursV[i * (laby.largeur + 1) + j] == true)
        ligne(out, j * CELLSIZE, i * CELLSIZE, j * CELLSIZE, (i + 1) * CELLSIZE,
              "black", 1);
  }

  for (int i = 0; i < laby.hauteur + 1; i++) {
    for (int j = 0; j < laby.largeur; j++)
      if (laby.mursH[i * laby.largeur + j] == true)
        ligne(out, j * CELLSIZE, i * CELLSIZE, (j + 1) * CELLSIZE, i * CELLSIZE,
              "black", 1);
  }

  Maillon *tmp;
  Maillon *maillon;
  for (int i = 0; i < mat.ordre; i++) {
    tmp = mat.lignes[i];
    if (tmp != nullptr) {
      maillon = tmp->suiv;
      while (maillon != nullptr) {
        if(tmp->l < maillon->l && tmp->c == maillon->c){//en bas
          ligne(out,(tmp->c * CELLSIZE) + CELLSIZE/2 ,(tmp->l * CELLSIZE) + CELLSIZE/2 , (tmp->c * CELLSIZE) +  CELLSIZE/2, (tmp->l +1 ) * CELLSIZE, "red",1);
          
        }
        if(tmp->l > maillon->l && tmp->c == maillon->c){//haut
         ligne(out,(tmp->c * CELLSIZE) + CELLSIZE/2 ,(tmp->l * CELLSIZE) + CELLSIZE/2 , (tmp->c * CELLSIZE) + CELLSIZE/2, tmp->l * CELLSIZE, "red",1);
        }
        if(tmp->c < maillon->c && tmp->l == maillon->l){//droite
          ligne(out,(tmp->c * CELLSIZE) + CELLSIZE/2 , (tmp->l * CELLSIZE) + CELLSIZE/2 ,(tmp->c+1) * CELLSIZE,(tmp->l * CELLSIZE) + CELLSIZE/2, "red",1);
        }
        if(tmp->c > maillon->c && tmp->l == maillon->l){//gauche
          ligne(out,(tmp->c  * CELLSIZE) + CELLSIZE/2 ,(tmp->l * CELLSIZE) + CELLSIZE/2 ,CELLSIZE * tmp->c ,(tmp->l * CELLSIZE) + CELLSIZE/2, "red",1);
        }

        maillon = maillon->suiv;
      }
     
    }
  }

  fermerFichierSVG(out);
}

void saisirCoordonnees(coordonnee &deb, coordonnee &fin, int largeur, int hauteur){

  while( deb.x < 0 || deb.x >= largeur  ){
    cout<<"Coodonnées sur les abscisses du début entre 0 et "<<largeur-1<<" : ";
    cin>>deb.x;
  }
  while(deb.y < 0 || deb.y >= hauteur ){
    cout<<"Coodonnées sur les ordonnées du début entre 0 et "<<hauteur-1<<" : ";
    cin>>deb.y;
  }

  while( fin.x < 0 || fin.x >= largeur ){
    cout<<"Coodonnées sur les abscisses du fin entre 0 et "<<largeur-1<<" : ";
    cin>>fin.x;
  }
  while(fin.y < 0 || fin.y >= hauteur ){
    cout<<"Coodonnées sur les ordonnées du fin entre 0 et "<<hauteur-1<<" : ";
    cin>>fin.y;
  }

}


// ----------------------------------------------------------------
// Fonctions liées aux parcours en largeur 
// ----------------------------------------------------------------


void initialiserFifo(Fifo &file){
  file.in = nullptr;
  file.out = nullptr;
}

bool estVide(Fifo file){
  if(file.in == nullptr && file.out == nullptr) return true;
  else return false;
}

void ajouter(Fifo &file, int v){
  MaillonEntier * tmp = file.in;
  MaillonEntier * insere = new MaillonEntier;
  insere->valeur = v;
  insere->suiv = nullptr ;
  insere->prec = nullptr;

  if(estVide(file)){//la liste est vide !!
    file.in = insere;
    file.out = insere ;
  }
  else{
    MaillonEntier *tmp = file.in;
    file.in = insere;
    insere->suiv = tmp;
    tmp->prec = insere ;
  }
}

void afficheListe(Fifo file){
  if( !estVide(file)){
    MaillonEntier * tmp = file.in;
    while( tmp!= file.out ){
      cout<<tmp->valeur<< " ";
      tmp = tmp->suiv;
    }
  cout<<tmp->valeur;
  }
  else cout<<"Liste vide !"<<endl;

}

int retirer(Fifo &file){
  if(!estVide(file)){
      MaillonEntier * tmp = file.out;
      if( file.out == file.in){//s'il contient un seul element
        file.out = nullptr;
        file.in = nullptr;
      }
      if(tmp != nullptr){
        file.out = tmp->prec;
        tmp->prec = nullptr;
      }
      else{
        file.out = nullptr;
      }
      int suppr = tmp->valeur;
      delete tmp;
      return suppr;
  }
 return -100;
}

//chemin calculerChemin(const MatriceAdjacence &mat, coordonnee deb, coordonnee fin, int largeur);
void parcoursEnLargeur(MatriceAdjacence mat, int sommetDepart,int sommetArrive,Couleur *coul, int *dist, int *parent,int largeur ){
    // Initialisations de tous les sommets !
    coul = new Couleur[mat.ordre];
  for(int i = 0 ; i < mat.ordre ; i++){
    coul[i] = BLANC;
    dist[i] = INFINI;
    parent[i] = INDEFINI;
  }
  // initialiserle sommet de dÃ©part
  coul[sommetDepart] = GRIS;
  dist[sommetDepart] = 0;
  parent[sommetDepart] = INDEFINI;

  Fifo mafile;
  initialiserFifo(mafile);
  ajouter(mafile,sommetDepart);

  while(!estVide(mafile)){
    int val = retirer(mafile);
    if( val != -100){
      Maillon *tmp = mat.lignes[val];
      while(tmp != nullptr){
        if(coul[tmp->l * largeur + tmp->c] == BLANC){
          coul[tmp->l * largeur + tmp->c] = GRIS;
          dist[tmp->l * largeur + tmp->c] = dist[val] + 1;
          parent[tmp->l * largeur + tmp->c] = val;
          ajouter(mafile, tmp->l * largeur + tmp->c);
        }
        tmp = tmp->suiv;
      }
      coul[val] = NOIR;
    }
  }
  afficheCouleurs(mat,coul);
  afficheDistances(mat,dist);
  afficheParents(mat,parent);
}

 chemin calculerChemin(const MatriceAdjacence &mat, coordonnee deb, coordonnee fin, int largeur){
   
    Couleur * coul = new Couleur [mat.ordre];
    int * dist = new int[mat.ordre];
    int * parent = new int[mat.ordre];
    int sommetDepart = deb.y * largeur + deb.x;
   int sommetArrive = fin.y * largeur + fin.x;
    parcoursEnLargeur(mat,sommetDepart,sommetArrive,coul,dist,parent,largeur);
   chemin chemins;
   
   chemins.lg = dist[sommetArrive] + 1;
   chemins.etape = new coordonnee[chemins.lg ];

  remplirTabChemin(sommetArrive,parent,chemins.etape,chemins.lg,largeur );
    return chemins;
}

void dessinerSolution(const labyrinthe &laby, const chemin &ch, const string &nomFichier){
   ofstream out;

  ouvrirFichierSVG(nomFichier, out, laby.largeur * CELLSIZE, laby.hauteur * CELLSIZE);
  rect(out, 0, 0, laby.largeur * CELLSIZE, laby.hauteur * CELLSIZE, "white");

  for (int i = 0; i < laby.hauteur; i++) {
    for (int j = 0; j < laby.largeur + 1; j++)
      if (laby.mursV[i * (laby.largeur + 1) + j] == true)
        ligne(out, j * CELLSIZE, i * CELLSIZE, j * CELLSIZE, (i + 1) * CELLSIZE,
              "black", 1);
  }

  for (int i = 0; i < laby.hauteur + 1; i++) {
    for (int j = 0; j < laby.largeur; j++)
      if (laby.mursH[i * laby.largeur + j] == true)
        ligne(out, j * CELLSIZE, i * CELLSIZE, (j + 1) * CELLSIZE, i * CELLSIZE,
              "black", 1);
  }

 int debx ,deby , finx , finy ;
  
  for(int i = 0 ; i < ch.lg - 1 ; i++){
    debx =  ch.etape[i].x;
    deby = ch.etape[i].y;

    finx =  ch.etape[i+1].x;
    finy = ch.etape[i+1].y;

    if(debx < finx && deby == finy){//en bas
      ligne(out,(deby * CELLSIZE) + CELLSIZE/2 ,(debx * CELLSIZE) + CELLSIZE/2 , (deby * CELLSIZE) +  CELLSIZE/2,  ((debx + 1  ) * CELLSIZE) + CELLSIZE/2, "red",1);
    }
    if(debx > finx && deby == finy){//haut
       ligne(out,(deby * CELLSIZE) + CELLSIZE/2 ,(debx * CELLSIZE) + CELLSIZE/2 , (deby * CELLSIZE) + CELLSIZE/2, (debx * CELLSIZE) - CELLSIZE/2  , "red",1);
    }
    if(deby < finy && debx == finx){//droite
        ligne(out,(deby * CELLSIZE) + CELLSIZE/2 , (debx * CELLSIZE) + CELLSIZE/2 ,((deby+1) * CELLSIZE) + CELLSIZE/2,(debx* CELLSIZE) + CELLSIZE/2, "red",1);
    }
    if(deby > finy && debx == finx){//gauche
        ligne(out,(deby  * CELLSIZE) + CELLSIZE/2 ,(debx * CELLSIZE) + CELLSIZE/2 ,(CELLSIZE * deby) - CELLSIZE/2  ,(debx * CELLSIZE) + CELLSIZE/2, "red",1);
    }
  }
  

  
  fermerFichierSVG(out);
}















void afficheCouleurs(MatriceAdjacence mat,Couleur *coul){
  cout<<"couleurs : ";
  for(int i = 0 ; i < mat.ordre ; i++){
    if(coul[i] == BLANC) cout<<" B ";
    if(coul[i] == GRIS) cout<<" G ";
    if(coul[i] == NOIR) cout<<" N ";
  }
  cout<<endl;
}

void afficheDistances(MatriceAdjacence mat,int *dist){
  cout<<"distances: ";
  for(int i = 0 ; i < mat.ordre ; i++){
    if(dist[i] == INFINI ) cout<<" X ";
    else cout<<" "<< dist[i]<<" ";
  }
  cout<<endl;
}

void afficheParents(MatriceAdjacence mat,int *parents){
  cout<<"parents  : ";
  for(int i = 0 ; i < mat.ordre ; i++){
    if(parents[i] == INDEFINI ) cout<<" X ";
    else cout<<" "<< parents[i]<<" ";
  }
  cout<<endl;
}

void afficherCheminVers(int sf, int *parent){
    if(sf != INDEFINI){
      afficherCheminVers(parent[sf], parent);
      cout<<sf<<" ";}
}
void remplirTabChemin(int sf, int *parent,coordonnee  *tab , int taille,int largeur  ){
    if(sf != INDEFINI ){
       remplirTabChemin(parent[sf], parent,tab,taille - 1 ,largeur);
       tab[taille - 1].x = sf/largeur ;
      tab[taille - 1].y = sf%largeur;  

      }
  
}


// --------------------------------------------------------------------
// définition des fonctions liées à l'écriture dans les fichiers SVG
// --------------------------------------------------------------------

bool ouvrirFichierSVG(const string &nomFichier, ofstream &out, int largeur,
                      int hauteur) {
  out.open(nomFichier);

  if (!out.is_open()) {
    cout << "erreur d'ouverture du fichier de dessin " << nomFichier << endl;
    return false;
  }

  // sortie de l'entête
  out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
  out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ";
  out << "width=\"" << largeur << "\" height=\"" << hauteur << "\""
      << " style=\"margin: " << MARGE << ";\">" << endl;

  return true;
}

void fermerFichierSVG(ofstream &out) {
  // fermeture de la balise svg
  out << "</svg>" << endl;

  out.close();
}

void text(ofstream &out, int x, int y, int size, const string &txt,
          const string &color) {

  out << "<text x=\"" << x << "\" y=\"" << y << "\"";
  out << " font-family=\"Verdana\" font-size=\"" << size << "\"";
  out << " text-anchor=\"middle\" ";
  out << " dominant-baseline=\"middle\" ";
  out << "fill=\"" << color << "\" >" << endl;
  out << txt << endl;
  out << "</text>" << endl;
}

void ligne(ofstream &out, int x1, int y1, int x2, int y2, const string &color,
           int width) {
  out << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\"";
  out << " x2=\"" << x2 << "\" y2=\"" << y2;
  out << "\" stroke=\"" << color << "\"";
  out << " stroke-width=\"" << width << "\""
      << " />" << endl;
}

void rect(ofstream &out, int x, int y, int width, int height,
          const string &color) {
  out << "<rect width=\"" << width << "\" height=\"" << height << "\"";
  out << " x=\"" << x << "\" y=\"" << y << "\" fill=\"" << color << "\"";
  out << " />" << endl;
}

// --------------------------------------------------------------------
// Fonction de conversion d'une valeur entière en chaîne de caractères
// --------------------------------------------------------------------

string intToString(int v) {
  stringstream s;
  s << v;
  return s.str();
}
