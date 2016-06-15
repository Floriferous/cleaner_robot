/* Nom: cercle.c
 * Description: Module cercle, s'occupe des opérations sur les cercles
 * Date: 21.05.2011
 * Auteur: Florian Bienefelt
 */


#include <stdlib.h>
#include <stdio.h>
#include "constantes.h"
#include "cercle.h"
#include <math.h>

#define FIN 6



/*--------------------------------------------------------------------*/
void cercle_verif_obst (CERCLE *obst , float nb_obst,
						float rayon_m, int *etat)
{
	int a,b;
	float alpha_11=0.,alpha_12=0.,alpha_01=0.,alpha_2=0.,alpha_02=0.;
	
	for(a=0;a<nb_obst;a++)
	{
		if( ((obst+a)->c.px + (obst+a)->r)> (rayon_m-R_BMIN) || 
			((obst+a)->c.px - (obst+a)->r)< R_BMIN )
		{
			printf("\nERREUR : obstacle (%f %f %f) est hors limites\n",
							(obst+a)->c.px,(obst+a)->r,(obst+a)->c.py);
			*etat=FIN;
		}
		
	//je rappelle que j'ai gardé les coordonnées polaires en mémoire
		
		alpha_01=asin(((obst+a)->r)/((obst+a)->c.px));
		alpha_11=((obst+a)->c.py) + alpha_01;		
		alpha_12=((obst+a)->c.py) - alpha_01;
		
		for(b=0;b<nb_obst;b++)
		{
			if(a!=b)
			{
				alpha_02 = asin(((obst+b)->r)/((obst+b)->c.px));
				alpha_2=((obst+b)->c.py) - alpha_02;
				
				if(fabs(alpha_2-alpha_11)<ANGLE_MIN)
				{
					printf("\nERREUR : intersection des obstacles : "
							"%f %f %f et %f %f %f\n"
							,(obst+a)->c.px,(obst+a)->r,(obst+a)->c.py,
							(obst+b)->c.px,(obst+b)->r,(obst+b)->c.py);
					*etat=FIN;
					return;
				}
				else
				{
					alpha_2+=2 * alpha_02;
					if(fabs(alpha_12-alpha_2)<ANGLE_MIN)
					{
						printf("\nERREUR : intersection des obstacles :"
							   " %f %f %f et %f %f %f\n",
							 (obst+a)->c.px,(obst+a)->r,(obst+a)->c.py,
							 (obst+b)->c.px,(obst+b)->r,(obst+b)->c.py);
						*etat=FIN;
						return;
					}
				}
			}
		}
	}
}



/*--------------------------------------------------------------------*/
//conversion de polaire à cartésien
void cercle_remp_coord(CERCLE *obst,float nb_obst)
{
	float alpha=0.,dist=0.;
	int a;
	
	for(a=0;a<nb_obst;a++)
	{
		alpha= (obst+a)->c.py;
		dist= (obst+a)->c.px;
		
		(obst+a)->c.px= dist * cos(alpha);
		(obst+a)->c.py= dist * sin(alpha);
	}
}



/*--------------------------------------------------------------------*/
void cercle_verif_part( POINT *part,float nb_part, CERCLE *obst,
							 float nb_obst,float rayon_m, int *etat)
{
	float part_x=0.,part_y=0.,dist_part=0.,
			obst_x=0., obst_y=0., dist_x=0.,dist_y=0.,dist=0.;
	int i,j;
	
	for(i=0;i<nb_part;i++)
	{
		part_x=(*(part+i)).px;
		part_y=(*(part+i)).py;
		dist_part=sqrt( part_x*part_x + part_y*part_y );
		if(dist_part>rayon_m)
		{
			printf("\nERREUR : particule(%f %f) pas dans le batiment\n",
														 part_x,part_y);
			*etat=FIN;
			return;
		}
		for(j=0;j<nb_obst;j++)
		{
			obst_x= (obst+j)->c.px;
			obst_y= (obst+j)->c.py;
			
			dist_x=obst_x-part_x;
			dist_y=obst_y-part_y;
			dist=sqrt( dist_x*dist_x + dist_y*dist_y );
			if( dist <= (obst+j)->r )
			{
				printf("\nERREUR : particule (%f %f) sur l'objet"
					   " de centre :%f %f et de rayon : %f)\n",
					   part_x,part_y,obst_x,obst_y,(obst+j)->r);
				*etat=FIN;
				return;
			}
		}
	}
}



/*--------------------------------------------------------------------*/
float calcul_distance(float x1, float y1, float x2, float y2)
{
	double dx = x2-x1, dy = y2-y1;
	return (float) sqrt(dx*dx + dy*dy);
}



/*--------------------------------------------------------------------*/
POINT decaler_interieur(POINT rob, float r_bat)
{
	
	float distance = calcul_distance(0,0, rob.px, rob.py);
	
	if( distance > r_bat - R_ROBOT)
	{
		float correction = (r_bat - R_ROBOT)/distance ;
		
		rob.px *= correction;
		rob.py *= correction;
	}
	return rob;
}



/*--------------------------------------------------------------------*/
POINT decaler_exterieur(POINT rob, CERCLE obst)
{
	float distance = calcul_distance(obst.c.px, obst.c.py, 
									 rob.px, rob.py);
	
	if(distance == 0)
	{
		printf("division par zéro\n");
		exit(EXIT_FAILURE);
	}
	else if( distance < obst.r + R_ROBOT)
	{
		float correction = (obst.r + R_ROBOT)/distance ;
		
		rob.px = obst.c.px + correction*(rob.px - obst.c.px);
		rob.py = obst.c.py + correction*(rob.py - obst.c.py);
	}
	return rob;
}
