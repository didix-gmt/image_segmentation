#!/bin/bash

# Script permettant de tester le programme de segmentation d'images sur des images de test (images/images_brut)

# Chemins des dossiers, du makefile et de l'exécutable
ROOT_DIR=$(dirname "$(dirname "$(realpath "$0")")") # Racine
IMAGE_DIR="$ROOT_DIR/images/images_brut"           # Dossier contenant les images
RESULTS_DIR="$ROOT_DIR/tests/results"              # Dossier pour les résultats
MAKEFILE="$ROOT_DIR/makefile"                      # Chemin du makefile
EXECUTABLE="$ROOT_DIR/segmentation"                # Exécutable

# Chronométrage : début
START_TIME=$(date +%s)

# Vérifier si le makefile existe
if [ ! -f "$MAKEFILE" ]; then
  echo "Erreur : makefile non trouvé à la racine du projet ($MAKEFILE)."
  exit 1
fi

# Compilation avec make
make -C "$ROOT_DIR"
if [ $? -ne 0 ]; then
  echo "Erreur lors de la compilation avec make."
  exit 1
fi

# Vérifier si l'exécutable segmentation a été généré
if [ ! -f "$EXECUTABLE" ]; then
  echo "Erreur : L'exécutable $EXECUTABLE n'a pas été généré."
  exit 1
fi

# Créer le dossier des résultats s'il n'existe pas
if [ ! -d "$RESULTS_DIR" ]; then
  echo "Création du dossier des résultats..."
  mkdir -p "$RESULTS_DIR"
fi

# Traitement de chaque image
echo "Traitement des images dans $IMAGE_DIR..."
for IMAGE_PATH in "$IMAGE_DIR"/*; do
  if [ -f "$IMAGE_PATH" ]; then
    IMAGE_NAME=$(basename "$IMAGE_PATH")           # Nom du fichier de l'image
    OUTPUT_PATH="$RESULTS_DIR/$IMAGE_NAME"         # Chemin de l'image segmentée

    echo "Segmentation de l'image : $IMAGE_NAME"

    # Exécution de l'outil de segmentation
    "$EXECUTABLE" -i "$IMAGE_PATH" -t "$OUTPUT_PATH"

    # Vérifier si le fichier des résultats a été créé
    if [ -f "$OUTPUT_PATH" ]; then
      echo "Image segmentée avec succès : $OUTPUT_PATH"
    else
      echo "Erreur : Le fichier de sortie $OUTPUT_PATH n'a pas été créé."
    fi
    echo "Traitement de l'image : $IMAGE_NAME terminé"
    echo " "
  fi
done

# Nettoyage avec make clean
make -C "$ROOT_DIR" clean
if [ $? -ne 0 ]; then
  echo "Erreur lors de l'exécution de make clean."
  exit 1
fi

# Chronométrage : fin
END_TIME=$(date +%s)
EXECUTION_TIME=$((END_TIME - START_TIME))

echo "Traitement terminé. Les résultats sont dans $RESULTS_DIR."
echo "Temps total d'exécution : $EXECUTION_TIME secondes."