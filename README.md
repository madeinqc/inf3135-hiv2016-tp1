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

## Contenu du projet

tp1.c - Code source du logiciel.
README.md - Ce fichier, une description du projet écrit en markdown.
Makefile - Fichier de configuration pour make.

## Status

Le projet est en développement et peut ne pas fonctionner ou contenir des bogues.
