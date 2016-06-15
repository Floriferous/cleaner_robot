/* Nom: parcours.h
 * Description: Module parcours: s'occupe de la trajectoire du robot
 * ainsi que du calcul de la surface parcourue
 * Date: 27.05.2011
 * Auteur: Florian Bienefelt
 */



#ifndef PARCOURS_H
#define PARCOURS_H




#include "cercle.h"




typedef struct Pos POS;




void parcours_ajouter (POS ** p_tete, POINT new);

void parcours_retirer( POS ** p_tete, POS *pos );

void parcours_vider( POS ** p_liste );

void parcours_draw(POS ** p_liste);
// s'occupe de calculer le parcours (liste chainée) et de le dessiner



int surface_create(float r, CERCLE * obst, int nb_obst);

void surface_update(POINT pos);

int surface_get();

void surface_destroy();
//approxime la surface totale du batiment avec 10000 particules



#endif
