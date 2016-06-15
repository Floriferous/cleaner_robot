/* Nom: parcours.c
 * Description: Module parcours: s'occupe de la trajectoire du robot
 * ainsi que du calcul de la surface parcourue
 * Date: 27.05.2011
 * Auteur: Florian Bienefelt
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graphic.h"
#include "cercle.h"
#include "constantes.h"



/*--------------------------------------------------------------------*/
typedef struct Pos POS;

struct Pos
{
	POINT position;
	
	POS * suivant;
};



/*--------------------------------------------------------------------*/
static int nb;
static POINT * surface;

static void draw( POS * pointeur );



/*--------------------------------------------------------------------*/
void parcours_ajouter (POS ** p_tete, POINT new)
{
	POS *pos = ( POS* )malloc( sizeof( POS ) );
	
	if(!pos)
	{
		printf("Erreur Malloc parcours\n");
		exit(EXIT_FAILURE);
	}

	pos->suivant = * p_tete;
	pos->position.px = new.px;
	pos->position.py = new.py;


	*p_tete = pos;
}



/*--------------------------------------------------------------------*/
void parcours_retirer( POS ** p_tete, POS *pos )
{
	POS* courant=* p_tete;
	POS* precedent=* p_tete;
	
	while ( courant != pos && courant != NULL )
	{
		precedent=courant;
		courant=courant->suivant;
	}
	
	if ( courant != NULL )
	{
		if ( courant != * p_tete )
		{
			precedent->suivant=courant->suivant;
			free ( courant );
		}
		else
		{
			* p_tete = courant->suivant;
			free ( courant );
		}
	}
}



/*--------------------------------------------------------------------*/
void parcours_vider( POS ** p_liste )
{
	POS *pos;
	
	while ( * p_liste != NULL )
	{
		pos = * p_liste;
		parcours_retirer ( p_liste, pos );
	}
}



/*--------------------------------------------------------------------*/
//fonction récursive qui dessine le parcours du robot
static void draw( POS * pointeur )
{
	if(pointeur->suivant)
	{
		graphic_set_color3f(0.7, 0.7, 0.7);
		graphic_set_line_width(0.6);
		graphic_draw_segment(pointeur->position.px, 
							 pointeur->position.py, 
							 pointeur->suivant->position.px, 
							 pointeur->suivant->position.py);
		
		draw(pointeur->suivant);
	}
}



/*--------------------------------------------------------------------*/
//vérifie les 2 pointeurs avant de commencer à dessiner le parcours
void parcours_draw(POS ** p_liste)
{
	if(p_liste && *p_liste)
		draw(*p_liste);
}



/*--------------------------------------------------------------------*/
//crée une surface qui est approximée par 100 000 points,
//si le robot passe dessus il les attrape comme des particules
//et je peux ainsi approximer la surface totale 
//avec une très bonne précision
int surface_create(float r, CERCLE * obst, int nb_obst)
{
	nb = 0;
	
	if(!(surface = malloc(100000 * sizeof(POINT))))
	{
		printf("ERROR MALLOC CREATE PART\n");
		exit(0);
	}
	
	int j = 0, i;
	bool ok;
		
	while(1)
	{
		float x = (2*r*((float) (rand() % 1000)/1000)) - r;
		float y = (2*r*((float) (rand() % 1000)/1000)) - r;
		
		ok = true;
		
		if(calcul_distance(0., 0., x, y) < r)
		{
			for(i=0; i<nb_obst; i++)
			{
				if(calcul_distance(obst[i].c.px, obst[i].c.py, x, y) < obst[i].r)
					ok = false;
			}
		}
		else
			ok = false;
		
		if(ok)
		{
			surface[j].px = x;
			surface[j].py = y;
			j++;
		}
			
		if(j == 100000)
			return EXIT_SUCCESS;	
	}
}



/*--------------------------------------------------------------------*/
//met la surface a jour en vérifiant si le robot est passé dessus
void surface_update(POINT pos)
{
	int i;
	
	for(i=0; i<100000; i++)
	{
		if(calcul_distance(pos.px, pos.py, surface[i].px, surface[i].py)
		   < R_ROBOT)
		{
			surface[i].px = R_BMAX + 1;
			surface[i].py = 0;
			nb++;
		}
	}
}



/*--------------------------------------------------------------------*/
//retourne le nb de points restants
int surface_get()
{
	return nb;
}



/*--------------------------------------------------------------------*/
void surface_destroy()
{
	if(surface)
	{
		free(surface);
		surface = NULL;
	}
}









