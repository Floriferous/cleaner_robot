/* Nom: robot.c
 * Description: Module robot: gère le robot
 * Date: 21.05.2011
 * Auteur: Florian Bienefelt
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#include "constantes.h"
#include "cercle.h"
#include "graphic.h"



/*--------------------------------------------------------------------*/
typedef struct Robot ROBOT;

struct Robot 
{ 
	POINT p ; 
	float orientation ; 
	float v_r_droite, v_r_gauche ; 
} ;



/*--------------------------------------------------------------------*/
//change la position du robot, il a de nouvelles vitesses en mémoire
void position_update(ROBOT * rob, float delta_t)
{
	double delta_g = rob->v_r_gauche * delta_t * R_ROUE;
	double delta_d = rob->v_r_droite * delta_t * R_ROUE;
	
	double delta_r = (delta_d + delta_g) / 2;
	double delta_a = (delta_d - delta_g) / (2*R_ROBOT);
	
	//vecteur de translation
	double vect_x = cos(rob->orientation);
	double vect_y = sin(rob->orientation);
	
	rob->p.px += delta_r * vect_x;
	rob->p.py += delta_r * vect_y;
	
	rob->orientation += delta_a;
	
	if(rob->orientation > M_PI)
		rob->orientation -= 2 * M_PI;
		
	if(rob->orientation < M_PI)
		rob->orientation += 2 * M_PI;
}



/*--------------------------------------------------------------------*/
ROBOT * robot_create()
{
	ROBOT * new;
	
	if(!(new = (ROBOT *) malloc(sizeof(ROBOT))))
	{
		printf("ERROR Malloc ROBOT\n");
		exit(0);
	}
	
	new->p.px = 0;
	new->p.py = 0;
	new->orientation = 0;
	new->v_r_droite = 0;
	new->v_r_droite = 0;
	
	
	return new;
}



/*--------------------------------------------------------------------*/
void robot_destroy(ROBOT * rob)
{
	if(rob)
	{
		free(rob);
		rob = NULL;
	}
}



/*--------------------------------------------------------------------*/
POINT robot_get_pos(ROBOT * rob)
{
	return rob->p;
}



/*--------------------------------------------------------------------*/
float robot_get_orient(ROBOT * rob)
{
	return rob->orientation;
}



/*--------------------------------------------------------------------*/
float robot_get_droite(ROBOT * rob)
{
	return rob->v_r_droite;
}



/*--------------------------------------------------------------------*/
float robot_get_gauche(ROBOT * rob)
{
	return rob->v_r_gauche;
}



/*--------------------------------------------------------------------*/
bool robot_set_pos(ROBOT * rob, POINT pos)
{
	rob->p = pos;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool robot_set_orient(ROBOT * rob, float orient)
{
	if(orient > 2*M_PI)
		orient -= 2*M_PI;
	
	
	rob->orientation = orient;
	
	return true;
}



/*--------------------------------------------------------------------*/
//j'interdis au robot d'avoir des vitesses négatives, ça lui permet
//d'avancer correctement et d'éviter qu'il tourne sur lui même
bool robot_set_droite(ROBOT * rob, float droite)
{
	if(droite < 0)
		rob->v_r_droite = 0;
	else if(droite > W_MAX)
		rob->v_r_droite = W_MAX;
	else
		rob->v_r_droite = droite;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool robot_set_gauche(ROBOT * rob, float gauche)
{
	if(gauche < 0)
		rob->v_r_gauche = 0;
	else if(gauche > W_MAX)
		rob->v_r_gauche = W_MAX;
	else
		rob->v_r_gauche = gauche;
	
	return true;
}



/*--------------------------------------------------------------------*/
//dessine le robot
void robot_draw(ROBOT * rob)
{
	graphic_set_color3f(1., 1, 1.);
	graphic_draw_circle(robot_get_pos(rob).px, 
						robot_get_pos(rob).py, 
						R_ROBOT, 1);
	
	
	graphic_set_color3f(1, 0., 0.);
	graphic_draw_circle(robot_get_pos(rob).px +
						R_ROBOT*cos(robot_get_orient(rob)), 
						robot_get_pos(rob).py +
						R_ROBOT*sin(robot_get_orient(rob)), 
						R_ROBOT/3, 1);	
}
