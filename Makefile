# Compilateur a utiliser :
##########################

CC     = gcc

# Options de compilation : (-g = pour deverminer)
#################################################

CFLAGS = -g


# Librairies a utiliser :
# LIBS: toutes les librairies qu'il faut intégrer
# dans le projet. Ne changez pas sauf -Llib -lglui qui définissent le chemin
# de la librairie de GLUI et le nom de la librairie GLUI. 
# Supposant vous avez un dossier bibliothek et le fichier libmeinGLUI.a (=la librairie de GLUI), 
# vous devez changez -Llib -lglui en -Lbibliothek -lmeinGLUI. C'est important que le nom de la
# librairie commence avec lib. Si vous n'avez plus la librairie de GLUI:
#   -Vous pouvez copier la librarie de /raunhardt/serie19/lib
#   -Refaire la librairie. Tous les fichiers se trouvent dans /raunhardt/serie18/required_files/
#    pour refaire il faut utiliser make.
#########################

LIBS   = -lglut -lGL -lGLU -lm -Llib -lglui -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXi

# Liste de fichiers source (.c) a compiler :
############################################

CFILES = parcours.c robot.c graphic.c projet.cpp simulation.c monde.c cercle.c

# Liste de modules objets (generee a partir de CFILES) :
# Les fichiers objets (.o) ont le même nom que les fichiers source (.c)
# Seulement le suffixe change.
########################################################

OFILES = $(CFILES:.c=.o)

# Nom du fichier executable :
#############################

CIBLE  = projet.x

# edition de liens (rassembler tous les objets -> executable)
#############################################################

projet.x: $(OFILES) 
	$(CC) $(OFILES) ${LIBS} -o $(CIBLE)


# compilation separee (production du module objet seulement)

graphic.o: graphic.c
	$(CC) -c graphic.c $(CFLAGS)

projet.o: $(GLUI_OBJS) projet.cpp 
	$(CC) -c projet.cpp $(CFLAGS)

robot.o: robot.c
	$(CC) -c robot.c $(CFLAGS)

monde.o: monde.c
	$(CC) -c monde.c $(CFLAGS)
	
simulation.o: simulation.c
	$(CC) -c simulation.c $(CFLAGS)
	
cercle.o: cercle.c
	$(CC) -c cercle.c $(CFLAGS)
	
parcours.o: parcours.c
	$(CC) -c parcours.c $(CFLAGS)



# Definitions de cibles particulieres :
#
# "make depend" : genere la liste des dependances
# "make clean"  : efface les fichiers .o et .x
#################################################

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x

#
# -- Regles de dependances generees par "make depend"
#####################################################
# DO NOT DELETE THIS LINE
parcours.o: parcours.c graphic.h cercle.h constantes.h
robot.o: robot.c constantes.h cercle.h graphic.h
graphic.o: graphic.c graphic.h
projet.o: projet.cpp glui.h graphic.h simulation.h constantes.h cercle.h \
 monde.h robot.h
simulation.o: simulation.c constantes.h cercle.h monde.h graphic.h \
 robot.h parcours.h simulation.h
monde.o: monde.c constantes.h cercle.h graphic.h
cercle.o: cercle.c constantes.h cercle.h
