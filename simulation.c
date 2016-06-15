/* Nom: simulation.c
 * Description: Module simulation: Chef d'orchestre du projet, ce module
 * coordine le robot avec le monde et fait avancer la simulation
 * Date: 27.05.2011
 * Auteur: Florian Bienefelt
 */




#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "constantes.h"
#include "cercle.h"
#include "monde.h"
#include "robot.h"
#include "graphic.h"
#include "parcours.h"
#include "simulation.h"



/*--------------------------------------------------------------------*/
struct Sim
{
	float delta_t;
	float temps;
	int nb_collis;
	int nb_part_actuel;
	ROBOT * rob;
	MONDE * monde;
	POS * parcours;
};



/*--------------------------------------------------------------------*/
//appelle la fonction de création d'obstacles 
//dans monde.c et réinitialise la surface couverte
void sim_create_obst(SIM * sim, int nb)
{
	monde_create_obst(sim->monde, nb);
	
	monde_adapt_part(sim->monde);
	
	sim_surface_create(sim);
}



/*--------------------------------------------------------------------*/
//idem
void sim_create_part(SIM * sim, int nb)
{
	monde_create_part(sim->monde, nb);
	
	sim->nb_part_actuel = nb;
	
	sim_surface_create(sim);
}



/*--------------------------------------------------------------------*/
//change les vitesses avec les les delta_vitesses en argument
void changer_vitesses(SIM * sim, float delta_droite, float delta_gauche)
{
	robot_set_droite(sim->rob, robot_get_droite(sim->rob) + 
					 delta_droite);
	robot_set_gauche(sim->rob, robot_get_gauche(sim->rob) + 
					 delta_gauche);
}



/*--------------------------------------------------------------------*/
//fonction qui met à jour la simulation
void sim_update(SIM * sim)
{
	//définit la variation maximale
	float delta_max = DELTA_W_MAX * sim->delta_t;
	
	//booléen qui fait que la vitesse ne change qu'une seule fois
	//par passage dans cette fonction
	bool change = true;
	
	
	position_update(sim->rob, sim->delta_t);
	surface_update(robot_get_pos(sim->rob));
	
	int i;
	CERCLE obst_i;
	
	//position avant possible décalage du robot 
	//par le monde ou des obstacles
	POINT pos_ini = robot_get_pos(sim->rob);
	
	robot_set_pos(sim->rob, decaler_interieur(robot_get_pos(sim->rob), 
							monde_get_rayon(sim->monde)) );
	
	for(i=0; i<monde_get_nb_obst(sim->monde);  i++)
	{
		obst_i = monde_get_obst(sim->monde)[i];
		robot_set_pos(sim->rob, decaler_exterieur
								(robot_get_pos(sim->rob), obst_i));
	}
	
	//position apres possible décalage du robot
	POINT pos_final = robot_get_pos(sim->rob);
	
	//ajoute la position actuelle au parcours
	parcours_ajouter(&sim->parcours, robot_get_pos(sim->rob));
	
	//si les 2 positions ne sont plus les memes, augmentation du 
	//nb de collision et changement de vitesses
	if(pos_ini.px != pos_final.px || pos_ini.py != pos_final.py)
	{
		sim->nb_collis++;
		
		//change la vitesse considérablement si une collision 
		//est détectée pour redresser la trajectoire
		//ne fonctionne pas avec un delta_t trop grand	
		if(sim->delta_t < 0.6)
		{									  
			changer_vitesses(sim, -delta_max/3.5, delta_max/3.5);

			change = false;
		}
	}
	
	
	//mise a jour des particules
	sim->nb_part_actuel = part_update(sim->monde, 
									  robot_get_pos(sim->rob), 
									  sim->nb_part_actuel);
	sim->temps += sim->delta_t;



	// mise a jour des vitesses	si aucune collision
	
	//si la somme des vitesses est trop basse, acceleration uniforme
	if(change && ((robot_get_droite(sim->rob) + 
				   robot_get_gauche(sim->rob)) < W_MAX*1.2))
	{
		changer_vitesses(sim, delta_max/2, delta_max/2);
		
		change = false;
	}
	
	//si l'écart entre les 2 vitesses est trop grand, 
	//elles se rattrappent pour assurer une belle trajectoire
	//ne fonctionne pas bien pour un delta_t trop grand
	if(sim->delta_t < 0.8 && change && fabs(robot_get_droite(sim->rob) - 
									   robot_get_gauche(sim->rob)) > 
									   W_MAX/3.5)
	{
		if(robot_get_droite(sim->rob) > robot_get_gauche(sim->rob) )
			changer_vitesses(sim, -delta_max/3,	delta_max/3);			
		else
			changer_vitesses(sim, delta_max/3,	-delta_max/3);
			
		change = false;
	}
	else if(change)
	{
		//sinon, ajout aléatoire de vitesses
		
		//calcul aléatoire de 2 delta_vitesses
		float droite = delta_max*((float) (rand() % 1000)/1000);
		float gauche = delta_max*((float) (rand() % 1000)/1000);
		
		//booléens aléatoires qui décident comment changer les vitesses
		bool bool1 = rand() % 2;
		bool bool2 = rand() % 2;
		
		if(bool1 && bool2)
			changer_vitesses(sim, droite, gauche);
		else if(bool1 && !bool2)
			changer_vitesses(sim, -droite, gauche);
		else if(!bool1 && bool2)
			changer_vitesses(sim, droite, -gauche);
		else
			changer_vitesses(sim, -droite, -gauche);
	}
	
}



/*--------------------------------------------------------------------*/
POINT sim_robot_get_pos(SIM * sim)
{
	return robot_get_pos(sim->rob);
}



/*--------------------------------------------------------------------*/
void sim_surface_create(SIM * sim)
{
		surface_create(monde_get_rayon(sim->monde),
					   monde_get_obst(sim->monde),
					   monde_get_nb_obst(sim->monde));
}



/*--------------------------------------------------------------------*/
int sim_surface_get(SIM * sim)
{
	return surface_get();
}



/*--------------------------------------------------------------------*/
float sim_monde_get_rayon(SIM * sim)
{
	return monde_get_rayon(sim->monde);
}



/*--------------------------------------------------------------------*/
int sim_monde_get_nb_part(SIM * sim)
{
	return monde_get_nb_part(sim->monde);
}



/*--------------------------------------------------------------------*/
SIM * sim_create()
{
	SIM * new;
	
	if(!(new = (SIM *) malloc(sizeof(SIM))))
	{
		printf("ERROR Malloc SIM\n");
		exit(0);
	}
	
	new->monde = monde_create();
	new->rob = robot_create();	
	new->parcours = NULL;
	
	
	return new;
}



/*--------------------------------------------------------------------*/
void sim_destroy(SIM * sim)
{
	robot_destroy(sim->rob);
	monde_destroy(sim->monde);
	parcours_vider(&sim->parcours);
	surface_destroy();
	if(sim)
	{
		free(sim);
		sim = NULL;
	}
}



/*--------------------------------------------------------------------*/
float sim_get_deltat(SIM * sim)
{
	return sim->delta_t;
}



/*--------------------------------------------------------------------*/
int sim_get_nb_part_actuel(SIM * sim)
{
	return sim->nb_part_actuel;
}



/*--------------------------------------------------------------------*/
float sim_get_temps(SIM * sim)
{
	return sim->temps;
}



/*--------------------------------------------------------------------*/
ROBOT * sim_get_rob(SIM * sim)
{
	return sim->rob;
}



/*--------------------------------------------------------------------*/
MONDE * sim_get_monde(SIM * sim)
{
	return sim->monde;
}



/*--------------------------------------------------------------------*/
int sim_get_nb_collis(SIM * sim)
{
	return sim->nb_collis;
}



/*--------------------------------------------------------------------*/
bool sim_set_deltat(SIM * sim, float delta_t)
{
	sim->delta_t = delta_t;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool sim_set_nb_part_actuel(SIM * sim, int nb)
{	
	if(nb <= 0)
		return false;
	
	
	sim->nb_part_actuel = nb;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool sim_set_temps(SIM * sim, float temps)
{
	if(temps > MAX_DURATION)
		return false;
	
	if(temps < 0)
	{
		printf("temps négatif impossible !!\n");
		exit(0);
	}
	
	sim->temps = temps;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool sim_set_rob(SIM * sim, ROBOT * rob)
{
	sim->rob = rob;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool sim_set_monde(SIM * sim, MONDE * monde)
{
	sim->monde = monde;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool sim_set_nb_collis(SIM * sim, int nb)
{
	if(nb < 0)
	{
		printf("collisions negatives ?!\n");
		exit(0);
	}
	
	sim->nb_collis = nb;
	
	return true;
}



/*--------------------------------------------------------------------*/
//dessine toute la simulation
void sim_draw(SIM * sim)
{
	//n'est effectué que si la simulation a un rayon du monde
	//assure que le dessin n'est pas effectué au lancement du programme
	if(monde_get_rayon(sim->monde))
	{
		graphic_set_color3f(0., 1., 1.);
		particules_draw(sim->monde);
		
		graphic_set_color3f(0., 0., 1.);
		obstacles_draw(sim->monde);
		
		graphic_set_color3f(1., 1., 1.);
		graphic_draw_circle(0, 0, monde_get_rayon(sim->monde), 0);
		
		parcours_draw(&sim->parcours);
		
		robot_draw(sim_get_rob(sim));
	}
}



/*--------------------------------------------------------------------*/
//retourne la simulation avec le nom du fichier passé en argument et
//initialise les différents champs de la structure simulation
SIM * sim_get(char * tab)
{
	SIM * sim = sim_create();
	
	sim->delta_t = monde_get(tab, sim->monde);
	
	if(!sim->delta_t)
		return NULL;
		
	sim_set_temps(sim, 0.);
	
	sim_set_nb_collis(sim, 0);
	
	sim_set_nb_part_actuel(sim, monde_get_nb_part(sim->monde) );
	
	return sim;
}
