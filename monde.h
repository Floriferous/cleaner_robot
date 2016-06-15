/* Nom: monde.h
 * Description: Module monde, s'occupe du monde, ses obstacles 
 * et particules, ainsi que de la lecture des fichiers
 * Date: 21.05.2011
 * Auteur: Florian Bienefelt
 */


#ifndef MONDE_H
#define MONDE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#include "constantes.h"
#include "cercle.h"
#include "graphic.h"

//structure contenant toutes les infos sur le monde
typedef struct Monde MONDE;




int part_update(MONDE * monde, POINT pos, int nb);
//met à jour les particules, avec la position du robot en argument



int monde_create_obst(MONDE * monde, int nb);

int monde_create_part(MONDE * monde, int nb);

void monde_adapt_part(MONDE * monde);




MONDE * monde_create();

void monde_destroy(MONDE * monde);




float monde_get_rayon(MONDE * monde);

CERCLE * monde_get_obst(MONDE * monde);

POINT * monde_get_part(MONDE * monde);

int monde_get_nb_obst(MONDE * monde);

int monde_get_nb_part(MONDE * monde);




bool monde_set_rayon(MONDE * monde, float r);

bool monde_set_nb_part(MONDE * monde, int nb);

bool monde_set_nb_obst(MONDE * monde, int nb);

bool monde_set_obst(MONDE * monde, CERCLE * obst);

bool monde_set_part(MONDE * monde, POINT * part);




int particules_draw(MONDE * monde);

int obstacles_draw(MONDE * monde);
//dessin des particules et obstacles


/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/

void ferreur(char * text,int *etat);


void lecture(char chaine[100],int *etat,float *delta_t,
			 int inf_gen[3], CERCLE **obst, POINT **part, 
			 int *i, MONDE * monde);

//lecture des informations sous forme d'integer

int lecture_int(char chaine[100], int inf_gen[3], int statement,
				int *etat, CERCLE **obst, POINT **part, MONDE * monde);

//allocation pour les obstacles

CERCLE * crea_tab_obst(int nb_obst, int *etat);

//allocation pour les particules

POINT * crea_tab_part(int nb_part, int *etat);

//enregistrement des coordonnées des obstacles

void lecture_obstacle(CERCLE *obst, int *i, char chaine[100],
								   int *etat, int inf_gen[3]);

//enregistrement des coordonnées des particules

void lecture_particule(POINT *part, int * i, char chaine[100],
					   int * etat, int inf_gen[3], CERCLE *obst);



float monde_get(char * tab, MONDE * monde);
//lit un fichier, stocke toutes les informations dans la variable monde
//et renvoie le delta t du fichier pour etre stocké dans une variable
//de type SIM


#endif

