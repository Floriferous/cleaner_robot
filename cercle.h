/* Nom: cercle.h
 * Description: Module cercle, s'occupe des opérations sur les cercles
 * Date: 21.05.2011
 * Auteur: Florian Bienefelt
 */


#ifndef CERCLE_H
#define CERCLE_H

// symboles 
#define X	0
#define Y	1

// types
struct Point
{
	float px, py;
};
typedef struct Point POINT;


struct Cercle
{
	POINT c;
	float r;

};
typedef struct Cercle CERCLE;




//vérifie si l'angle minimum entre les 
//obstacles est respecté et si l'obstacle n'est pas hors limites

void cercle_verif_obst(CERCLE *obst, float nb_obst,
					   float rayon_m, int *etat);

//remplace les coordonnées polaires en coordonnées cartésiennes

void cercle_remp_coord(CERCLE *obst, float nb_obst);

//vérifie si les particules sont 
//dans le batiment et pas sur des obstacles

void cercle_verif_part(POINT *part, float nb_part, 
					   CERCLE *obst, float nb_obst, 
					   float rayon_m, int *etat);



float calcul_distance(float x1, float y1, float x2, float y2);


//corrige les coordonnées de robot 
//si celui-ci à une intersection avec un obstacle 

POINT decaler_interieur(POINT rob, float r_bat);

//corrige les coordonnées de robot si celui-ci est en dehors du batiment

POINT decaler_exterieur(POINT rob, CERCLE obst);

#endif
