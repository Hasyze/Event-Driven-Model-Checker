TARGET = main

CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++11

# Répertoire contenant les fichiers sources (.cc)
SRC_DIR = .

# Répertoire contenant les fichiers d'en-tête (.h)
# INCLUDE_DIR = .

# Liste des fichiers sources (.cc)
SOURCES = $(wildcard $(SRC_DIR)/*.cc)

# Liste des fichiers objets générés (.o)
OBJECTS = $(SOURCES:.cc=.o)

# Cible par défaut : générer l'exécutable
all: $(TARGET)

# Règle de compilation pour générer les fichiers objets
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle de génération de l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Cible pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJECTS) $(TARGET)
