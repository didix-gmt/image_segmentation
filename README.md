# Projet de Traitement d'Images

Bienvenue dans le dépôt du projet de programmation de **Tom BELOT** et **Didier GAMIETTE**.

## Structure du projet

- `images/`  : Contient les images de test pour le programme de traitement d'images.
- `include/` : Contient les fichiers d'en-tête du projet.
  - `extern` : Contient les fichiers d'en-tête d'origine extérieur (écrits par autrui) du projet.
- `sources/` : Contient le code source C du projet.
- `tests/`   : Contient le code source testant les fonctions implémentées ainsi qu'un script test.sh exécutant le programme sur des fichiers de test.
  - `results/` : Contient les images générées après traitement.
- `docs/`     : Contient les fichiers pour le rapport, la documentation et les sources bibliographiques. La documentation devra être générée avec la commande `make doc` et sa page d'accueil sera située à `docs/documentation/index.html`

## Prérequis

Veuillez installer les librairies suivantes: 

- La librairie doxygen:

```bash
sudo apt update
sudo apt install doxygen
```

## Makefile

Ce projet utilise deux Makefile pour automatiser le processus de compilation. Le Makefile situé à la racine du projet contient une règle principale `segmentation` pour utiliser la segmentation d'images, ainsi que plusieurs autres règles pour gérer les différentes parties du projet. Un autre Makefile situé dans le dossier `tests` permet de compiler l'exécutable des tests des fonctions implémentées. Voici un aperçu des règles définies dans le Makefile :

### Règles principales

#### Makefile à la racine

- all : Règle par défaut qui compile l'exécutable principal, toutes les dépendances nécessaires ainsi que la documentation du code source.

- segmentation : Exécutable principal permettant la segmentation d'images.

- doc : Règle permettant de créer la documentation du code source C. La documentation ainsi créée se situe dans le dossier `docs/documentation/` et possède comme page principal `index.html`.

- clean : Supprime les fichiers générés lors de la compilation (tels que les exécutables) et la documentation, permettant ainsi de nettoyer le répertoire de travail.

#### Makefile du répertoire tests

- all : Règle par défaut qui compile l'exécutable principal de test et toutes les dépendances nécessaires.

- tests_fonctions : Exécutable principal effectuant des tests sur les différentes fonctions implémentées. Cette règle permet de vérifier le bon fonctionnement des fonctionnalités du projet.

- clean : Supprime les fichiers générés lors de la compilation (tels que les exécutables), permettant ainsi de nettoyer le répertoire de travail.

## Utilisation du programme

Le programme devra d'abord être compilé via la règle `make all` permettant également de générer la documentation du code source. L'exécutable principal `segmentation` permet de réaliser la segmentation d'images et s'utiliser à l'aide d'extensions :

- -i CHEMIN (obligatoire) : Indique au programme le chemin de l'image à segmenter.

- -t FICHIER (obligatoire) : Indique au programme le résultat de l'image segmentée dans FICHIER.

- -l LIMITE (optionnel) : Interdit la segmentation si une des dimensions de l'image est supérieur à LIMITE.

- -s (optionnel) : Affiche en console des statistiques sur le graphe de l'image.

- -f (optionnel) : Affiche en console les chemins segmentants du graphe de l'image.

Voici un exemple d'utilisation du programme :

```bash
make all
./segmentation -i CHEMIN -t FICHIER -l LIMITE -s -f
make clean
```

## Fonctionnalités non disponibles :