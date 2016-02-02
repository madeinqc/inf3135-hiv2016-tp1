/*
 * Ce programme utilise les données de GeoNames pour lire en mémoire
 * les pays et les villes au dessus de 15000 habitants.
 * Une fois chargés en mémoire, les données sont trié en ordre
 * decroissant avec un algorythme de tri rapide. On affiche alors les
 * n villes les plus populeuses où n est un nombre entre 1 et 5000
 * passé en paramètre.
 *
 * La structure d'affichage est la suivante:
 * Rang   Nom                       Pays                       Population
 * ----   ---                       ----                       ----------
 *    1   Shanghai                  China                        22315474
 */
// TODO Renommer tout avec des noms français
// TODO Vérifier les constantes

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ERREUR_NOMBRE_ARGUMENTS -2
#define ERREUR_ARGUMENT_HORS_BORNES -3
#define IMPOSSIBLE_OUVRIR_FICHIER -4
#define ARGUMENT_MINIMUM 1
#define ARGUMENT_MAXIMUM 5000
#define LONG_NOM_PAYS 201
#define LONG_CODE_ISO_PAYS 3
#define LONG_NOM_VILLE 201
#define LONG_MESSAGE_ERREUR 50
#define NOMBRE_PAYS_MAXIMUM 300
#define NOMBRE_VILLE_MAXIMUM 25000
#define LONG_LIGNE 11000
#define FICHIER_PAYS "data/countryInfo.txt"
#define FICHIER_VILLE "data/cities15000.txt"

// Les structures et constantes associées suivantes (Pays et Ville) sont inspirées de l'énoncé du travail disponible à http://thales.math.uqam.ca/~blondin/fr/inf3135-tp1
struct Pays {
  char nom[LONG_NOM_PAYS]; // Le nom ASCII
  char codeISO[LONG_CODE_ISO_PAYS]; // Le code ISO du pays
};

struct Ville {
  char nom[LONG_NOM_VILLE]; // Le nom ASCII
  long population; // La population
  struct Pays *pays; // Le pays de la ville
};

void afficherErreur(char *msg);
void afficherUtilisation(char *nomDuProgramme);
int obtenirPays(struct Pays *pays);
int extractionPays(struct Pays *pays, char *ligne);
void passerjetons(char **ligne, int nombre, char *jeton);
int obtenirVille(struct Ville* villes, struct Pays pays[], int nombrePays);
int extractionVille(struct Ville *ville, char* ligne, struct Pays pays[], int nombrePays);
void remplirPays(struct Ville *ville, char *codeISO, struct Pays pays[], int nombrePays);
void trierRapidement(struct Ville tab[], int longTab);
void trierRapidementRec(struct Ville tab[], int i, int j);
int partitionner(struct Ville tab[], int i, int j);

int main(int argc, char** argv) {
  if (argc != 2) {
    afficherErreur("Vous devez fournir un argument.");
    afficherUtilisation(argv[0]);
    return ERREUR_NOMBRE_ARGUMENTS;
  }

  char *fin;
  long count = strtol(argv[1], &fin, 10);
  if (*fin != 0 || count < ARGUMENT_MINIMUM || count > ARGUMENT_MAXIMUM) {
    char message[LONG_MESSAGE_ERREUR];
    
    afficherErreur("Vous devez fournir un nombre valide en argument.");
    afficherUtilisation(argv[0]);
    return ERREUR_ARGUMENT_HORS_BORNES;
  }

  struct Pays pays[NOMBRE_PAYS_MAXIMUM];
  int nombrePays = obtenirPays(pays);

  struct Ville villes[NOMBRE_VILLE_MAXIMUM];
  int nombreVilles = obtenirVille(villes, pays, nombrePays);

  trierRapidement(villes, nombreVilles);

  printf("%4s   %-25s   %-25s   %10s\n",
      "Rang", "Nom", "Pays", "Population");
  printf("%4s   %-25s   %-25s   %10s\n",
      "----", "---", "----", "----------");
  int i;
  for (i = 0; i < count && i < nombreVilles; i++) {
    printf("%4d   %-25.25s   %-25.25s   %10d\n",
        i+1, villes[i].nom, villes[i].pays->nom, villes[i].population);
  }

  return 0;
}

/**
 * Charge les pays dans le pointeur passé en paramètre.
 * @param pays Le pointeur qui sera populé avec la liste des pays.
 * @return Nombre de pays chargés ou le numéro d'erreur en cas d'échec.
 */
int obtenirPays(struct Pays pays[]) {
  FILE* file = fopen(FICHIER_PAYS, "r");
  if (!file) {
    char message[LONG_MESSAGE_ERREUR] = "Impossible d'ouvrir le fichier ";
    strncat(message, FICHIER_PAYS, LONG_MESSAGE_ERREUR);
    afficherErreur(message);
    return IMPOSSIBLE_OUVRIR_FICHIER;
  }

  char ligne[LONG_LIGNE];
  int nombrePaysLus = 0;
  
  while (fgets(ligne, LONG_LIGNE, file) != NULL && nombrePaysLus < NOMBRE_PAYS_MAXIMUM) {
    if (extractionPays(&pays[nombrePaysLus], ligne)) ++nombrePaysLus;
  }

  fclose(file);

  return nombrePaysLus;
}

/**
 * Extrait les informations de la ligne passé en paramètre et remplis la structure Pays.
 * @param pays Le pointeur vers un Pays qui sera populé.
 * @param ligne La chaîne de caractère contenant l'information à extraire.
 * @return Retourne le nombre de pays lus.
 */
int extractionPays(struct Pays *pays, char* ligne) {
  char *jeton;

  if (ligne[0] == '#') return 0;

  jeton = strsep(&ligne, "\t");
  if (jeton == NULL) return 0;
  strncpy(pays->codeISO, jeton, LONG_CODE_ISO_PAYS - 1);
  pays->codeISO[LONG_CODE_ISO_PAYS - 1] = 0;

  passerjetons(&ligne, 3, "\t");
  
  jeton = strsep(&ligne, "\t");
  if (jeton == NULL) return 0;
  strncpy(pays->nom, jeton, LONG_NOM_PAYS - 1);
  pays->nom[LONG_NOM_PAYS - 1] = 0;

  return 1;
}

/**
 * Permet de passer un nombre déterminé de jeton (jeton) dans une string à l'aide de strsep.
 * @param ligne La chaine de caractère dans laquelle passer les jetons.
 * @param nombre Nombre de jetons à passer.
 * @param jeton Les jetons valide.
 */
void passerjetons(char **ligne, int nombre, char *jeton) {
  int i;
  for (i = 0; i < nombre; i++) strsep(ligne, jeton);
}

/**
 * Charge les villes dans le pointeur passé en paramètre.
 * @param villes Le pointeur qui sera populé avec la liste des villes.
 * @param pays La liste des pays utilisé pour populer les villes.
 * @param nombrePays Le nombre de pays dans la liste des pays.
 * @return Nombre de villes chargées ou le numéro d'erreur en cas d'échec.
 */
int obtenirVille(struct Ville* villes, struct Pays pays[], int nombrePays) {
  FILE* file = fopen(FICHIER_VILLE, "r");
  if (!file) {
    char message[LONG_MESSAGE_ERREUR] = "Impossible d'ouvrir le fichier ";
    strncat(message, FICHIER_VILLE, LONG_MESSAGE_ERREUR);
    afficherErreur(message);
    return IMPOSSIBLE_OUVRIR_FICHIER;
  }

  char ligne[LONG_LIGNE];
  int nombreVillesLus = 0;
  
  while (fgets(ligne, LONG_LIGNE, file) != NULL && nombreVillesLus < NOMBRE_VILLE_MAXIMUM) {
    if (extractionVille(&villes[nombreVillesLus], ligne, pays, nombrePays)) ++nombreVillesLus;
  }

  fclose(file);

  return nombreVillesLus;
}

/**
 * Extrait les informations de la ligne passé en paramètre et remplis la structure Ville.
 * @param ville Le pointeur vers une Ville qui sera populé.
 * @param ligne La chaîne de caractère contenant l'information à extraire.
 * @param pays La liste des pays utilisé pour populer le pays de la ville.
 * @param nombrePays Le nombre de pays dans la liste des pays.
 * @return Retourne une valeur non nulle lorsque les données sont bien chargés, sinon on 0 est retourné.
 */
int extractionVille(struct Ville *ville, char* ligne, struct Pays pays[], int nombrePays) {
  char *jeton;

  passerjetons(&ligne, 2, "\t");

  jeton = strsep(&ligne, "\t");
  if (jeton == NULL) return 0;
  strncpy(ville->nom, jeton, LONG_NOM_VILLE - 1);
  ville->nom[LONG_NOM_VILLE - 1] = 0;

  passerjetons(&ligne, 5, "\t");

  jeton = strsep(&ligne, "\t");
  if (jeton == NULL) return 0;

  remplirPays(ville, jeton, pays, nombrePays);
  
  passerjetons(&ligne, 5, "\t");
  
  jeton = strsep(&ligne, "\t");
  if (jeton == NULL) return 0;
  char *fin;
  ville->population = strtol(jeton, &fin, 10);

  return 1;
}

/**
 * Remplit le Pays de la Ville passé en paramètre avec le pays possédant le code ISO correspondant au code passé en paramètre.
 * @param ville Le pointeur vers une Ville qui aura son pays populé.
 * @param codeISO Le code ISO du Pays à assigner.
 * @param pays La liste de tous les Pays chargés.
 * @param nombrePays Le nombre de pays contenu dans le tableau des pays.
 */
void remplirPays(struct Ville *ville, char *codeISO, struct Pays pays[], int nombrePays) {
  int i;
  for (i = 0; i < nombrePays; i++) {
    if (strncmp(codeISO, pays[i].codeISO, 2) == 0) {
      ville->pays = &pays[i];
      return;
    }
  }
}

/**
 * Affiche un message d'erreur reçu en paramètre.
 * @param char* msg Le message d'erreur à afficher.
 */
void afficherErreur(char* msg) {
  printf("Erreur: %s\n\n", msg);
}

/**
 * Affiche l'information d'utilisation du logiciel.
 * @param char* Nom du programme.
 */
void afficherUtilisation(char* nomDuProgramme) {
  printf("Utilisation:\n\t%s <nombre de villes>\n\nOù <nombre de ville> doit être un entier entre %d et %d.\n", nomDuProgramme, ARGUMENT_MINIMUM, ARGUMENT_MAXIMUM);
}

/**
 * Effectue un tri d'un tableau de Ville dans l'ordre décroissant basé sur la population des villes.
 * @param tab Tableau de villes à ordonner.
 * @param longTab Longueur du tableau de villes.
 */
void trierRapidement(struct Ville tab[], int longTab) {
  trierRapidementRec(tab, 0, longTab - 1);
}

/**
 * Effectue un tri des élément entre i et j d'un tableau de Ville dans l'ordre décroissant basé sur la population des villes.
 * @param tab Tableau de villes à ordonner.
 * @param i Index inférieur à partir duquel les éléments seront triés.
 * @param j Index suppérieur à partir duquel les éléments seront triés.
 */
void trierRapidementRec(struct Ville tab[], int i, int j) {
  int pivot;
  if (i < j) {
    pivot = partitionner(tab, i, j);
    trierRapidementRec(tab, i, pivot - 1);
    trierRapidementRec(tab, pivot + 1, j);
  }
}

/**
 * Tri le tableau de ville entre les indexes i et j selon un pivot et retourne un nouveau pivot pour la suite du tri.
 * @param tab Tableau de villes à ordonner.
 * @param i Index inférieur à partir duquel les éléments seront triés.
 * @param j Index suppérieur à partir duquel les éléments seront triés.
 */
int partitionner(struct Ville tab[], int i, int j) {
  struct Ville pivot = tab[j];
  int min = i;
  int max;
  for (max = i; max < j; max++) {
    if (tab[max].population >= pivot.population) {
      struct Ville temp = tab[min];
      tab[min] = tab[max];
      tab[max] = temp;
      min++;
    }
  }
  struct Ville temp = tab[min];
  tab[min] = tab[j];
  tab[j] = temp;
  return min;
}
