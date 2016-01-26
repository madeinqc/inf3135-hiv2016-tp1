#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARG_COUNT_ERROR -2
#define ARG_RANGE_ERROR -3
#define COULD_NOT_OPEN_FILE -4
#define LONG_NOM_PAYS 201
#define CODE_ISO_PAYS 3
#define LONG_NOM_VILLE 201
#define MESSAGE_ERREUR_MAXIMUM 50
#define NOMBRE_PAYS_MAXIMUM 300
#define NOMBRE_VILLE_MAXIMUM 25000
#define LONG_LIGNE 11000
#define FICHIER_PAYS "data/countryInfo.txt"
#define FICHIER_VILLE "data/cities15000.txt"

// Les structures et constantes associées suivantes (Pays et Ville) sont inspirées de l'énoncé du travail disponible à http://thales.math.uqam.ca/~blondin/fr/inf3135-tp1
struct Pays {
  char nom[LONG_NOM_PAYS]; // Le nom ASCII
  char codeISO[CODE_ISO_PAYS]; // Le code ISO du pays
};

struct Ville {
  char nom[LONG_NOM_VILLE]; // Le nom ASCII
  long population; // La population
  struct Pays pays; // Le pays de la ville
};

void printError(char *msg);
void printUsage(char *nomDuProgramme);
int obtenirPays(struct Pays *pays);
int extractionPays(struct Pays *pays, char *line);
void passerTokens(char **line, int nombre, char *token);
int obtenirVille(struct Ville* villes, struct Pays pays[], int nombrePays);
int extractionVille(struct Ville *ville, char* line, struct Pays pays[], int nombrePays);
void remplirPays(struct Ville *ville, char *codeISO, struct Pays pays[], int nombrePays);
void trierRapidement(struct Ville tab[], int longTab);
void trierRapidementRec(struct Ville tab[], int i, int j);
int partitionner(struct Ville tab[], int i, int j);

int main(int argc, char** argv) {
  if (argc != 2) {
    printError("Vous devez fournir un argument.");
    printUsage(argv[0]);
    return ARG_COUNT_ERROR;
  }

  char *fin;
  long count = strtol(argv[1], &fin, 10);
  if (*fin != 0 || count < 1 || count > 5000) {
    printError("Vous devez fournir un nombre entre 1 et 5000 comme argument.");
    printUsage(argv[0]);
    return ARG_RANGE_ERROR;
  }

  struct Pays pays[NOMBRE_PAYS_MAXIMUM];
  int nombrePays = obtenirPays(pays);

  struct Ville villes[NOMBRE_VILLE_MAXIMUM];
  int nombreVilles = obtenirVille(villes, pays, nombrePays);

  trierRapidement(villes, nombreVilles);

  printf("%4s   %-25s   %-25s   %10s\n", "Rang", "Nom", "Pays", "Population");
  printf("%4s   %-25s   %-25s   %10s\n", "----", "---", "----", "----------");
  int i;
  for (i = 0; i < count && i < nombreVilles; i++) {
    printf("%4d   %-25.25s   %-25.25s   %10d\n", i+1, villes[i].nom, villes[i].pays.nom, villes[i].population);
  }

  return 0;
}

/**
 * Charge les pays dans le pointeur passé en paramètre.
 * @param pays Le pointeur qui sera populé avec la liste des pays.
 * @return Nombre de pays chargé ou le numéro d'erreur en cas d'échec.
 */
int obtenirPays(struct Pays pays[]) {
  FILE* file = fopen(FICHIER_PAYS, "r");
  if (!file) {
    char message[MESSAGE_ERREUR_MAXIMUM] = "Impossible d'ouvrir le fichier ";
    strncat(message, FICHIER_PAYS, MESSAGE_ERREUR_MAXIMUM);
    printError(message);
    return COULD_NOT_OPEN_FILE;
  }

  char line[LONG_LIGNE];
  int nombrePaysLus = 0;
  
  while (fgets(line, LONG_LIGNE, file) != NULL && nombrePaysLus < NOMBRE_PAYS_MAXIMUM) {
    if (extractionPays(&pays[nombrePaysLus], line)) ++nombrePaysLus;
  }

  fclose(file);

  return nombrePaysLus;
}

/**
 * Extrait les informations de la ligne passé en paramètre et remplis la structure Pays.
 * @param pays Le pointeur vers un Pays qui sera populé.
 * @param line La chaîne de caractère contenant l'information à extraire.
 */
int extractionPays(struct Pays *pays, char* line) {
  char *token;

  if (line[0] == '#') return 0;

  token = strsep(&line, "\t");
  if (token == NULL) return 0;
  strncpy(pays->codeISO, token, CODE_ISO_PAYS - 1);
  pays->codeISO[CODE_ISO_PAYS - 1] = 0;

  passerTokens(&line, 3, "\t");
  
  token = strsep(&line, "\t");
  if (token == NULL) return 0;
  strncpy(pays->nom, token, LONG_NOM_PAYS - 1);
  pays->nom[LONG_NOM_PAYS - 1] = 0;

  return 1;
}

void passerTokens(char **line, int nombre, char *token) {
  int i;
  for (i = 0; i < nombre; i++) strsep(line, token);
}

int obtenirVille(struct Ville* villes, struct Pays pays[], int nombrePays) {
  FILE* file = fopen(FICHIER_VILLE, "r");
  if (!file) {
    char message[MESSAGE_ERREUR_MAXIMUM] = "Impossible d'ouvrir le fichier ";
    strncat(message, FICHIER_VILLE, MESSAGE_ERREUR_MAXIMUM);
    printError(message);
    return COULD_NOT_OPEN_FILE;
  }

  char line[LONG_LIGNE];
  int nombreVillesLus = 0;
  
  while (fgets(line, LONG_LIGNE, file) != NULL && nombreVillesLus < NOMBRE_VILLE_MAXIMUM) {
    if (extractionVille(&villes[nombreVillesLus], line, pays, nombrePays)) ++nombreVillesLus;
  }

  fclose(file);

  return nombreVillesLus;
}

int extractionVille(struct Ville *ville, char* line, struct Pays pays[], int nombrePays) {
  char *token;

  passerTokens(&line, 2, "\t");

  token = strsep(&line, "\t");
  if (token == NULL) return 0;
  strncpy(ville->nom, token, LONG_NOM_VILLE - 1);
  ville->nom[LONG_NOM_VILLE - 1] = 0;

  passerTokens(&line, 5, "\t");

  token = strsep(&line, "\t");
  if (token == NULL) return 0;

  remplirPays(ville, token, pays, nombrePays);
  
  passerTokens(&line, 5, "\t");
  
  token = strsep(&line, "\t");
  if (token == NULL) return 0;
  char *fin;
  ville->population = strtol(token, &fin, 10);

  return 1;
}

void remplirPays(struct Ville *ville, char *codeISO, struct Pays pays[], int nombrePays) {
  int i;
  for (i = 0; i < nombrePays; i++) {
    if (strncmp(codeISO, pays[i].codeISO, 2) == 0) {
      ville->pays = pays[i];
      return;
    }
  }
}

/**
 * Affiche un message d'erreur reçu en paramètre.
 * @param char* msg Le message d'erreur à afficher.
 */
void printError(char* msg) {
  printf("Erreur: %s\n\n", msg);
}

/**
 * Affiche l'information d'utilisation du logiciel.
 * @param char* Nom du programme.
 */
void printUsage(char* nomDuProgramme) {
  printf("Usage:\n\t%s <nombre de villes>\n\nOù <nombre de ville> doit être un entier entre 1 et 5000.\n", nomDuProgramme);
}

void trierRapidement(struct Ville tab[], int longTab) {
  trierRapidementRec(tab, 0, longTab-1);
}

void trierRapidementRec(struct Ville tab[], int i, int j) {
  int pivot;
  if (i < j) {
    pivot = partitionner(tab, i, j);
    trierRapidementRec(tab, i, pivot - 1);
    trierRapidementRec(tab, pivot + 1, j);
  }
}

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
