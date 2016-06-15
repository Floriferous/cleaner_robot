/* Nom: robot.h
 * Description: Module robot: gère le robot
 * Date: 21.05.2011
 * Auteur: Florian Bienefelt
 */



#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>
#include <stdbool.h>

#include "constantes.h"
#include "cercle.h"
#include "graphic.h"



//structure qui contient toutes les données caractérisant le robot
typedef struct Robot ROBOT;




void position_update(ROBOT * rob, float delta_t);
//met a jour la position du robot grace a ses nouvelles vitesses



ROBOT * robot_create();

void robot_destroy(ROBOT * rob);




POINT robot_get_pos(ROBOT * rob);

float robot_get_orient(ROBOT * rob);

float robot_get_droite(ROBOT * rob);

float robot_get_gauche(ROBOT * rob);




bool robot_set_pos(ROBOT * rob, POINT pos);

bool robot_set_orient(ROBOT * rob, float orient);

bool robot_set_droite(ROBOT * rob, float droite);

bool robot_set_gauche(ROBOT * rob, float gauche);




void robot_draw(ROBOT * rob) ;
//dessine le robot

#endif
