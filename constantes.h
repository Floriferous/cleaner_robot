/* Nom: constantes.h
 * Description: Contient les constantes utilisées par tout le projet
 * Date: 17.02.2011
 * Version: V2
 */

#ifndef CONSTANTES_H
#define CONSTANTES_H

#define R_ROBOT			0.2f		//Rayon du robot 
#define R_ROUE			0.05f 		//Rayon de la roue 
#define R_BMIN			(4*R_ROBOT)	//Rayon min du bâtiment
#define R_BMAX			8.f		    //Rayon max du bâtiment
#define R_OMIN			R_ROBOT		//Rayon min d'un obstacle
#define ANGLE_MIN		0.25f		//Angle min entre deux obstacles
#define MAX_DURATION	3600.f	    //Durée max pour nettoyer
#define W_MAX			(1/R_ROUE)	//Vitesse angulaire max des roues
#define DELTA_W_MAX		W_MAX		//Variation/seconde max des roues

#endif
