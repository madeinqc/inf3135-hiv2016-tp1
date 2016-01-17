#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARG_COUNT_ERROR -2
#define ARG_RANGE_ERROR -3

void printError(char* msg);
void printUsage(char* nomDuProgramme);

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

  // TODO Implémenter la fonctionnalité principale

  return 0;
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
