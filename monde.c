/* Nom: monde.c
 * Description: Module monde, s'occupe du monde, ses obstacles 
 * et particules, ainsi que de la lecture des fichiers
 * Date: 27.05.2011
 * Auteur: Florian Bienefelt
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#include "constantes.h"
#include "cercle.h"
#include "graphic.h"

/*--------------------------------------------------------------------*/

enum Etat_lect {DELTA_T,RAYON_B , NB_OBST, LECT_OBST,
							NB_PART, LECT_PART ,FIN};


/*--------------------------------------------------------------------*/
static void ferreur(char * text,int *etat);
static CERCLE * crea_tab_obst(int nb_obst,int *etat);
static POINT * crea_tab_part(int nb_part,int *etat);


/*--------------------------------------------------------------------*/
struct Monde 
{ 
	float r;
	int nb_obst, nb_part;
	CERCLE * obst; 
	POINT * part; 
} ;

typedef struct Monde MONDE;



/*--------------------------------------------------------------------*/
//adapte les particules au nouveau monde, si on change les obstacles
//ça les décale vers l'extérieur
void monde_adapt_part(MONDE * monde)
{
		int i, j;
	
	for(i=0; i<monde->nb_obst; i++)
	{
		for(j=0; j<monde->nb_part; j++)
		{
			monde->part[j] = decaler_exterieur(monde->part[j], 
											   monde->obst[i]);
		}
	}
}



/*--------------------------------------------------------------------*/
//fonction qui crée les particules de façon aléatoires
int monde_create_part(MONDE * monde, int nb)
{
	if(monde->part)
	{
		free(monde->part);
		monde->part = NULL;
	}
	
	if(!nb)
	{
		monde->nb_part = 0;
		monde->part = NULL;
		return EXIT_SUCCESS;
	}
	
	monde->nb_part = nb;
	
	if(!(monde->part = malloc(nb * sizeof(POINT))))
	{
		printf("ERROR MALLOC CREATE PART\n");
		exit(0);
	}
	
	int i, j = 0;
	int ok;
		
	while(1)
	{
		ok = 1;
		
		float x = (2*monde->r*((float) (rand() % 1000)/1000))- monde->r;
		float y = (2*monde->r*((float) (rand() % 1000)/1000))- monde->r;
		
		if(calcul_distance(0., 0., x, y) < monde->r)
		{
			for(i = 0; i<monde->nb_obst; i++)
			{
				if(calcul_distance(monde->obst[i].c.px, 
								   monde->obst[i].c.py, x, y) < 
								   monde->obst[i].r)
				{
					ok = 0;
					break;
				}
			}
			
		}
		else
			ok = 0;
		
		//si les tests sont passés, la particule est ajoutée
		if(ok)
			{
				monde->part[j].px = x;
				monde->part[j].py = y;
				j++;
			}
			
		if(j == nb)
			return EXIT_SUCCESS;
			
	}
}



/*--------------------------------------------------------------------*/
int monde_create_obst(MONDE * monde, int nb)
{

	if(monde->obst)
	{
		free(monde->obst);
		monde->obst = NULL;
	}
	
	if(!nb)
	{
		monde->nb_obst = 0;
		monde->obst = NULL;
		return EXIT_SUCCESS;
	}
	
	monde->nb_obst = nb;
	
	if(!(monde->obst = malloc(nb * sizeof(CERCLE))))
	{
		printf("ERROR MALLOC CREATE OBST\n");
		exit(0);
	}
	
	int id = 0, i;
	bool ok;
	float distance;
	float alpha;
	float rayon;
	float r_max = ((2*M_PI - nb * ANGLE_MIN)/ nb);

	if(r_max > (monde->r -2*R_BMIN / 2))
		r_max = (monde->r - 2*R_BMIN) / 2;
	
	while(1)
	{
		ok = true;
		
		distance = (monde->r - 2*R_BMIN - 2*r_max)*
				   ((float) (rand() % 1000)/1000) + R_BMIN + r_max;
		alpha = 2*M_PI*((float) (rand() % 1000)/1000) - M_PI;
		rayon = r_max*((float) (rand() % 1000)/1000);
		
		if(rayon < R_OMIN)
			rayon = r_max;
		
		if((distance + rayon) > (monde->r-R_BMIN) || 
		   (distance - rayon) < R_BMIN )
			ok = false;
		
		//angle entre le centre et la tangente menant a l'obstacle
		float angle = fabs(asin(rayon/distance));
		
		
		if(ok)
		{
			for(i=0; i<id; i++)
			{
				//distance du centre de l'obstacle i	
				float dist_i = sqrt(pow(monde->obst[i].c.px, 2) + 
									pow(monde->obst[i].c.py, 2));
				
				//angle entre le centre et la tangente 
				//menant a l'obstacle i
				float angle_i = fabs(asin(monde->obst[i].r/dist_i));
				float alpha_i = atan2(monde->obst[i].c.py, 
									  monde->obst[i].c.px); 
				
				//calcule l'angle de la section et l'ajuste 
				//pour correctement obtenir le plus petit
				float angle_section = 
					  fabs(alpha - alpha_i);
				if(angle_section > M_PI)
					angle_section = fabs(angle_section - 2*M_PI);
				
				//vérifie si tous les angles ont 
				//la place dans la section
				if(angle_section < angle + angle_i + ANGLE_MIN)
				{
					ok = false;
					break;
				}
			}
		}
		
		if(ok)
		{
			monde->obst[id].c.px = distance * cos(alpha);
			monde->obst[id].c.py = distance * sin(alpha);
			monde->obst[id].r = rayon;
			id++;
		}
			
		if(id == nb)
			break;	
	}
	return EXIT_SUCCESS;
}



/*--------------------------------------------------------------------*/
//fonction qui enleve les particules du monee 
//si elles sont sous le robot
int part_update(MONDE * monde, POINT pos, int nb)
{
	int i;
	float dist;
	for(i=0; i<monde->nb_part; i++)
	{
		dist = calcul_distance(monde->part[i].px, monde->part[i].py, 
							   pos.px, pos.py);
		
		if(dist < R_ROBOT)
		{
			//éloigne la particule radicalement du monde si elle est
			//nettoyée et n'est plus dessinée de par son écart
			monde->part[i].px = 10000;
			monde->part[i].py = 0.;
			nb--;
		}
	
	}
	return nb;
}



/*--------------------------------------------------------------------*/
MONDE * monde_create()
{
	MONDE * new;
	
	if(!(new = (MONDE *) malloc(sizeof(MONDE))))
	{
		printf("ERROR Malloc MONDE\n");
		exit(0);
	}
	
	new->r = 0.;
	
	return new;
}



/*--------------------------------------------------------------------*/
void monde_destroy(MONDE * monde)
{
	if(monde)
	{
		if(monde->obst)
		{
			free(monde->obst);
			monde->obst = NULL;
		}
			
		if(monde->part)
		{
			free(monde->part);
			monde->part = NULL;
		}
		
		
		free(monde);
		monde = NULL;
	}
}



/*--------------------------------------------------------------------*/
float monde_get_rayon(MONDE * monde)
{
	return monde->r;
}



/*--------------------------------------------------------------------*/
int monde_get_nb_obst(MONDE * monde)
{
	return monde->nb_obst;
}



/*--------------------------------------------------------------------*/
int monde_get_nb_part(MONDE * monde)
{
	return monde->nb_part;
}



/*--------------------------------------------------------------------*/
CERCLE * monde_get_obst(MONDE * monde)
{
	return monde->obst;
}



/*--------------------------------------------------------------------*/
POINT * monde_get_part(MONDE * monde)
{
	return monde->part;
}



/*--------------------------------------------------------------------*/
bool monde_set_rayon(MONDE * monde, float r)
{
	if(r > R_BMAX || r < R_BMIN)
		return false;
	
	monde->r = r;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool monde_set_nb_part(MONDE * monde, int nb)
{
	if(nb < 0)
		return false;
	
	monde->nb_part = nb;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool monde_set_nb_obst(MONDE * monde, int nb)
{
	if(nb < 0)
		return false;
	
	monde->nb_obst = nb;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool monde_set_obst(MONDE * monde, CERCLE * obst)
{	
	if(obst)
		monde->obst = obst;
	else
		monde->obst = NULL;
	
	return true;
}



/*--------------------------------------------------------------------*/
bool monde_set_part(MONDE * monde, POINT * part)
{
	monde->part = part;
	
	return true;
}



/*--------------------------------------------------------------------*/
//dessine les particules
void particules_draw(MONDE * monde)
{
	graphic_set_color3f(0., 1., 1.);
	
	int i;
	
	for(i=0; i < monde->nb_part; i++)
	{
		graphic_draw_rectangle(monde->part[i].px, monde->part[i].py, 
							   R_ROBOT/5, R_ROBOT/5, 1);
	}
	
}



/*--------------------------------------------------------------------*/
//dessine les obstacles
void obstacles_draw(MONDE * monde)
{
	graphic_set_color3f(0., 0., 1.);
	
	int i;
	
	for(i=0; i < monde->nb_obst; i++)
	{
		graphic_draw_circle(monde->obst[i].c.px, monde->obst[i].c.py, 
							monde->obst[i].r, 1);
	}
}



//Je tiens à remercier Gilles Vetsch, à qui j'ai emprunté ce code:

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
static void ferreur(char * text, int *etat)
{
	printf("Erreur : %s\n", text);
	*etat=FIN;
}



/*--------------------------------------------------------------------*/
static CERCLE * crea_tab_obst(int nb_obst, int *etat)
{
	CERCLE * tab_obst=NULL;
	if(!(tab_obst=malloc(nb_obst * sizeof(CERCLE))))
		ferreur("malloc pour obstacle impossible\n",etat);
	return tab_obst;
}



/*--------------------------------------------------------------------*/
static POINT * crea_tab_part(int nb_part, int *etat)
{
	POINT * tab_part=NULL;
	if(!(tab_part=malloc(nb_part * sizeof(POINT))))
		ferreur("malloc pour particules impossible\n",etat);
	return tab_part;
}



/*--------------------------------------------------------------------*/
static int lecture_int(char chaine[100], int inf_gen[3], int statement,
								 int *etat, CERCLE **obst, POINT **part, 
								 MONDE * monde)
{
	if(statement<=3 && statement>=0)
	{
		if(sscanf(chaine,"%d",&inf_gen[statement]))
		{
			switch(*etat)
			{
				case RAYON_B:	
					if(inf_gen[0]<R_BMIN || inf_gen[0]>R_BMAX)
					{
						ferreur("rayon du batiment non conforme",etat);
						*etat=FIN;
					}
			
					monde->r = inf_gen[statement];
						*etat=NB_OBST;
					break;
				case NB_OBST:
					monde->nb_obst = inf_gen[1];
					if(inf_gen[1]==0)
					{	
						*etat=NB_PART;
					}
					else
					{
						*etat=LECT_OBST;
						*obst=crea_tab_obst(inf_gen[1],etat);
					}
					break;	
				case NB_PART:
					
					monde->nb_part = inf_gen[2];
					if(inf_gen[2]==0) *etat=FIN;
					else
					{
						*etat=LECT_PART;
						*part=crea_tab_part(inf_gen[2],etat);
					}
			}
			return 0;
		}
		else return 1;
	}
	else
	{
		ferreur("demande de lecture non comforme(inf_gen)\n",etat);
		*etat=FIN;
		return 1;
	}
}



/*--------------------------------------------------------------------*/
static void lecture_obstacle(CERCLE *obst, int *i, char chaine[100],
								   int *etat, int inf_gen[3], 
								   MONDE * monde)
{
	float dist=0.,rayon=0.,alpha=0.;
	
	/* je garde les coordonnées polaires pour pouvoir faire les calculs
	   d'intersection plus facilement, je changerai ensuite */
	if(sscanf(chaine,"%f %f %f",&dist,&rayon,&alpha))
	{		
		(obst+*i)->c.px=dist;
		(obst+*i)->c.py=alpha;
		(obst+*i)->r=rayon;
	}
	else
	{
		ferreur("lecture obstacles impossible",etat);
		*etat=FIN;
	}
		
	(*i)++;
	if(*i==inf_gen[1])
	{
		*i=0;
		*etat=NB_PART;
		cercle_verif_obst(obst , inf_gen[1],inf_gen[0],etat);
		cercle_remp_coord(obst , inf_gen[1]);
	}
	
	monde->obst = obst;
}



/*--------------------------------------------------------------------*/
static void lecture_particule(POINT *part, int * i, char chaine[100],
					   int * etat, int inf_gen[3], CERCLE *obst, 
					   MONDE * monde)
{
	float part_x=0., part_y=0.;
	
	if(sscanf(chaine,"%f %f",&part_x,&part_y))
	{
		(*(part+*i)).px=part_x;
		(*(part+*i)).py=part_y;
	}
	else
	{
		ferreur("lecture particules impossible",etat);
		*etat=FIN;
	}
		
	(*i)++;

	if(*i==inf_gen[2]) 
	{
		*i=0;
		cercle_verif_part(part, inf_gen[2], obst, inf_gen[1], 
						  inf_gen[0], etat);
	}
	
	monde->part = part;
}



/*--------------------------------------------------------------------*/
static void lecture(char chaine[100], int *etat, float *delta_t, 
					int inf_gen[3], CERCLE **obst, POINT **part, 
					int *i, MONDE * monde)
{
	switch(*etat)
	{	
		case DELTA_T: 
			if(sscanf(chaine,"%f",delta_t))
			{
				*etat=RAYON_B;
			}
			else 
			{
				ferreur("lecture delta t impossibe\n",etat);
				*etat=FIN;
			}
			break;
							
		case RAYON_B: 
			if(lecture_int(chaine,inf_gen,0,etat,NULL,NULL, monde))
			{
				ferreur("lecture rayon du batiment impossibe\n",etat);
				*etat=FIN;
			}
			break;
							
		case NB_OBST: 
			if(lecture_int(chaine,inf_gen,1,etat,obst,NULL, monde))
			{
				ferreur("lecture nombre d'obstacle impossibe\n",etat);
				*etat=FIN;
			}
			break;
						
		case LECT_OBST:	lecture_obstacle(*obst,i,chaine,etat,inf_gen, 
										 monde);
						break;
						
		case NB_PART: 
			if(lecture_int(chaine,inf_gen,2,etat,NULL,part, monde))
			{
			   ferreur("lecture nombre de particules impossibe\n",etat);
				*etat=FIN;
			}
			break;
						
		case LECT_PART:
			lecture_particule(*part,i,chaine,etat,inf_gen,*obst, monde);
			break;
	}
}



/*--------------------------------------------------------------------*/
float monde_get(char * text, MONDE * monde)
{
	FILE *fentree;
	char chaine[100];
	int inf_gen[3];
	float delta_t;
	int i=0, etat;
	
	if(!text)
		return 0;
		
	CERCLE * obst=NULL;

	POINT * part=NULL;
	


	etat=DELTA_T;
	for(i=0;i<3;i++) inf_gen[i]=0;
	i=0;

	fentree = fopen(text, "r");
	
	if(fentree==NULL)
	{
		ferreur("impossible d'ouvrir le fichier",&etat);
		return 0;
	}
	else
	{
		while(fgets(chaine,100,fentree))
		{
			if(chaine[0]=='#'|| chaine[0]=='\n'|| chaine[0]=='\r')
				continue;
			else
			  lecture(chaine, &etat, &delta_t, inf_gen, &obst,&part, &i, 
					  monde);
				
			if(etat==FIN) 
				return 0;
		}
		
		fclose(fentree);
	}
	
	

	return delta_t;
}
