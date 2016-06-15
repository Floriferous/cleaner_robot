/* Nom: simulation.h
 * Description: Module simulation: Chef d'orchestre du projet, ce module
 * coordine le robot avec le monde et fait avancer la simulation
 * Date: 27.05.2011
 * Auteur: Florian Bienefelt
 */




#ifndef SIMULATION_H
#define SIMULATION_H




#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>




#include "constantes.h"
#include "cercle.h"
#include "monde.h"
#include "robot.h"
#include "graphic.h"




typedef struct Sim SIM;




void sim_create_obst(SIM * sim, int nb);

void sim_create_part(SIM * sim, int nb);

void sim_surface_create(SIM * sim);
//fonctions qui Bonus qui créent des particules, obstacles, et la
//surface qui est approximée par 10000 particules




void changer_vitesses(SIM *sim, float delta_droite, float delta_gauche);

void sim_update(SIM * sim);
//la fonction appelée par idle, met la simulation à jour pour un delta t



SIM * sim_create();

void sim_destroy(SIM * sim);




SIM * sim_get(char * tab);



float sim_get_deltat(SIM * sim);

int sim_get_nb_part_actuel(SIM * sim);

float sim_get_temps(SIM * sim);

ROBOT * sim_get_rob(SIM * sim);

MONDE * sim_get_monde(SIM * sim);

int sim_get_nb_collis(SIM * sim);




POINT sim_robot_get_pos(SIM * sim);

float sim_monde_get_rayon(SIM * sim);

int sim_monde_get_nb_part(SIM * sim);

int sim_surface_get(SIM * sim);




bool sim_set_deltat(SIM * sim, float delta_t);

bool sim_set_nb_part_actuel(SIM * sim, int nb);

bool sim_set_temps(SIM * sim, float temps);

bool sim_set_rob(SIM * sim, ROBOT * rob);

bool sim_set_monde(SIM * sim, MONDE * monde);

bool sim_set_nb_collis(SIM * sim, int nb);




void sim_draw(SIM * sim);
//dessine le monde

ROBOT * update_robot(ROBOT * rob) ;
//decide comment changer la vitesse des roués, 
//et change sa position suite à cela, retourne le robot

ROBOT * check_collisions(ROBOT * rob, MONDE * monde);
//vérifie si le robot n’est pas entré en collision avec le monde

POINT * update_part(POINT * rob, POINT * part) ;
//vérifie si le robot a nettoyé des particules, 
//avec le robot et le pointeur des particules en argument


MONDE * destroy_world(MONDE * monde);
//libère l’espace alloué du monde
#endif
