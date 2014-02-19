
# ############################################################################ #
#                                  MAKEFILE                                    #
# ############################################################################ #

# Définition des cibles particulières
.PHONY: check clean mrproper help

# ############################################################################ #
# ############################################################################ #


# Définition de variables :
#	# Compilateur
CC =			gcc

#	# Dossier contenant les sources
SRC_DIR = 		src
#	# Dossier où créer les fichiers objets
OBJ_DIR = 		obj
#	# Dossier où créer l'exécutable
BIN_DIR = 		bin

#	# Nom de l'exécutable
EXEC = 			class
#	# Noms des fichiers sources
SRCS =			$(wildcard $(SRC_DIR)/*.c)
#	# Noms des fichiers objets (génération dynamique)
OBJS =			$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#	# Declaration des options
CFLAGS = 		-Wall
#	# Declaration des librairies
LDFLAGS = 		-lsvm


# ############################################################################ #
# ############################################################################ #


# Enumération des cibles.

#	# Compile l'ensemble du projet
all: check $(OBJS)
	@echo "\033[1;32m"
	@echo "Fin des messages compilateur (syntaxe)."
	@echo
	@echo "======================================="
	@echo "\033[1;33m"
	@echo "Début des messages linker."
	@echo "\033[1;31m"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(BIN_DIR)/$(EXEC)
	@echo "\033[1;32m"
	@echo "Fin des messages linker."
	@echo
	@echo "======================================="
	@echo 
	@echo "Projet construit avec succès !"
	@echo "\033[0m"


#	# Crée un fichier objet à partir d'un fichier source
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo
	@echo "\033[1;33m  $<\n\033[1;31m"
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "\033[1;32m -> $@ créé."


# Enumération des cibles particulières.

#	# Supprime tous les fichier objets
clean :
	@echo "\033[1;33m"
	@rm -fv $(OBJ_DIR)/*.o
	@echo "\033[0m"

#	# Supprime tous les fichiers objets et l'exécutable
mrproper :
	@echo "\033[1;33m"
	@rm -fv $(OBJ_DIR)/*.o
	@rm -fv $(BIN_DIR)/$(EXEC)
	@echo "\033[0m"

#	# Affiche le ReadMe
help :
	@cat ReadMe.txt | less
	
#	# Vérifie l'arborescence
check :
	@if [ ! -e bin ]; then mkdir bin; fi
	@if [ ! -e obj ]; then mkdir obj; fi


# ############################################################################ #
# ############################################################################ #

