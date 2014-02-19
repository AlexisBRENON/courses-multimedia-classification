#############################################################################
  #                                                                       #  
  # BRENON Alexis                                                         #  
  # PISKOROWSKI Laura                                                     #
  #                                                                       #  
#############################################################################

                        	Classification d'images
                              

~~ Description ~~

Ce programme permet de classifier un ensemble d'image en utilisant un algorithme de séparation à vecteur porteur, en se basant sur la bibliothèque SVM.

~~ Compilation ~~

Pour compiler :
    ~$ cd classification
    ~/classification$ make

~~ Execution ~~

Pour exécuter, une fois la compilation réussie sans erreur, il vous suffit
de taper la commande suivante :
    ~/classification$ bin/class -i <inputFile>

~~ Documentation ~~

Un fichier de génération de documentation est fournis dans le dossier 
classification/doc.
Après avoir installé doxygen, entrez la commande suivante :
    ~/classification$ doxygen doc/doxyfile
    
Pour afficher la documentation résultante :
    ~/classification$ {firefox|opera} doc/html/index.html

~~ Contacts ~~

Laura PISKOROWSKI : laura.piskorowski@gmail.com
Alexis BRENON : brenon.alexis@gmail.com
