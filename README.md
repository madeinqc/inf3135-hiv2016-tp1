# Travail pratique 1

## Description

Ce projet affiche les villes les plus populeuses du monde selon les données de GeoNames. Le nombre de villes affichées est paramétrable et doit se situé entre 1 et 5000.
Ce travail est effectué dans le cadre du cours *INF3135 - Construction et maintenance de logiciels* donné à l'*Université du Québec à Montréal*.

## Auteur

Marc-Antoine Sauvé (SAUM13119008) - madeinqchw@gmail.com

## Fonctionnement

La compilation du logiciel se fait avec *make*.
Les données sont téléchargées ou mises à jour à l'aide de *make database*.

Le logiciel s'utilise comme ceci:

```
./tp1 <nombre de villes>
```
Où *<nombre de villes>* est un chiffre entre 1 et 5000 et représente le nombre de villes à afficher.

### Exemple 1

```
./tp1 20
Rang   Nom                         Pays                        Population
----   ---                         ----                        ----------
   1   Shanghai                    China                         22315474
   2   Buenos Aires                Argentina                     13076300
   3   Mumbai                      India                         12691836
   4   Mexico City                 Mexico                        12294193
   5   Beijing                     China                         11716620
   6   Karachi                     Pakistan                      11624219
   7   Istanbul                    Turkey                        11174257
   8   Tianjin                     China                         11090314
   9   Guangzhou                   China                         11071424
  10   Delhi                       India                         10927986
  11   Manila                      Philippines                   10444527
  12   Moscow                      Russia                        10381222
  13   Shenzhen                    China                         10358381
  14   Dhaka                       Bangladesh                    10356500
  15   Seoul                       South Korea                   10349312
  16   Sao Paulo                   Brazil                        10021295
  17   Wuhan                       China                          9785388
  18   Lagos                       Nigeria                        9000000
  19   Jakarta                     Indonesia                      8540121
  20   Tokyo                       Japan                          8336599
```

### Exemple 2

```
./tp1 test
Erreur: Vous devez fournir un nombre valide en argument.

Utilisation:
  ./tp1 <nombre de villes>

  Où <nombre de ville> doit être un entier entre 1 et 5000.
```

## Contenu du projet

tp1.c - Code source du logiciel.
README.md - Ce fichier, une description du projet écrit en markdown.
Makefile - Fichier de configuration pour make. Ce fichier contient les instructions nécessaires pour compiler le logiciel, pour nettoyer le dossier ainsi que pour télécharger les données nécessaire à l'exécution.

## Références

[Quicksort - Wikipedia, the free encyclopedia](https://en.wikipedia.org/wiki/Quicksort) - Le pseudocode à été utiliser pour écrire l'algorythme de tri rapide.
[Travail pratique 1 - Homepage of Alexandre Blondin Massé](http://thales.math.uqam.ca/~blondin/fr/inf3135-tp1) - Utilisation des structures de données Ville et Pays.

## Status

Le projet est terminé et fonctionnel. Aucun bogue détecté.
