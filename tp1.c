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

// Les structures et constantes associées suivantes (Pays et Ville) sont inspirées de l'énoncé du travail disponible à http://thales.math.uqam.ca/~blondin/fr/inf3135-tp1
typedef struct Pays {
  char nom[LONG_NOM_PAYS]; // Le nom ASCII
  char codeISO[CODE_ISO_PAYS]; // Le code ISO du pays
} Pays;
const Pays PAYS_BIDON = {"?", "?"};

typedef struct Ville {
  char nom[LONG_NOM_VILLE]; // Le nom ASCII
  long population; // La population
  Pays pays; // Le pays de la ville
} Ville;
const Ville VILLE_BIDON = {"?", -1, {"?", "?"}};

void printError(char* msg);
void printUsage(char* nomDuProgramme);
int obtenirPays(Pays* pays);
int extractionPays(Pays *pays, char* line);
int obtenirVille(Ville* villes);
int extractionVille(Ville *ville, char* line);

int main(int argc, char** argv) {
  if (argc != 2) {
    printError("Vous devez fournir un argument.");
    printUsage(argv[0]);
    return ARG_COUNT_ERROR;
  }

  int count = atoi(argv[1]);
  if (count < 1 || count > 5000) {
    printError("Vous devez fournir un nombre entre 1 et 5000 comme argument.");
    printUsage(argv[0]);
    return ARG_RANGE_ERROR;
  }

  Pays pays[NOMBRE_PAYS_MAXIMUM];
  int nombrePays = obtenirPays(pays);

  // TODO Implémenter la fonctionnalité principale

  return 0;
}

/**
 * Charge les pays dans le pointeur passé en paramètre.
 * @param pays Le pointeur qui sera populé avec la liste des pays.
 * @return Nombre de pays chargé ou le numéro d'erreur en cas d'échec.
 */
int obtenirPays(Pays pays[]) {
  FILE* file = fopen(FICHIER_PAYS, "r");
  if (!file) {
    char message[MESSAGE_ERREUR_MAXIMUM];
    sprintf(message, "Impossible d'ouvrir le fichier %s", FICHIER_PAYS);
    printError(message);
    return COULD_NOT_OPEN_FILE;
  }

  char line[LONG_LIGNE];
  int nombrePaysLus = 0;
  
  while (fgets(line, LONG_LIGNE, file) != NULL) {
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
int extractionPays(Pays *pays, char* line) {
  char *token;

  if (line[0] == '#')
    return 0;

  token = strsep(&line, "\t");
  if (line == NULL) return 0;
  strcpy(pays->codeISO, token);

  strsep(&line, "\t");
  strsep(&line, "\t");
  strsep(&line, "\t");
  
  token = strsep(&line, "\t");
  if (line == NULL) return 0;
  strcpy(pays->nom, token);

  return 1;
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
  printf("Usage:\n\t./%s <nombre de villes>\n\nOù <nombre de ville> doit être un entier entre 1 et 5000.\n", nomDuProgramme);
}
