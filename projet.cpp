/* Nom: projet.cpp
 * Description: Module C++: s'occupe de coordiner l'affichage avec
 * GLUI et GLUT
 * Date: 28.05.2011
 * Auteur: Florian Bienefelt
 */




#include   <stdio.h>
#include   <stdlib.h>
#include   <stdbool.h>
#include   <string.h>
#include   <GL/glu.h>
#include   <GL/glut.h>
#include   <time.h>
#include   "glui.h"
#include <unistd.h>

extern "C"  
{
    #include   "graphic.h"
    #include   "simulation.h"
}



/*--------------------------------------------------------------------*/
namespace 
{
	int main_window;
	
	//variables des checkbox
	int zoomed, stepmode, accelerated;
	
	//variables qui déterminent le glOrtho, sont changées dans
	//display et reshape
	GLfloat x_min, x_max, y_min, y_max, aspect_ratio = 1;
	
	//variable active la simulation si elle est à 1
	bool simulate;
	SIM * sim = NULL;
	
	GLUI_EditText   *filename;
	GLUI_EditText   *elapsed;
	GLUI_EditText   *collision;
	GLUI_EditText   *cleaned;
	GLUI_EditText   *total;
	GLUI_EditText   *status;
	GLUI_EditText   *obst;
	GLUI_EditText   *part;
	GLUI_EditText   *surface;
	GLUI_Button     *load;
	GLUI_Button     *start;
	GLUI_Button     *paus;
	GLUI_Button     *step;
	GLUI_Button     *quit;
	GLUI_Checkbox   *stepb;
	GLUI_Checkbox   *zoom;
	GLUI_Checkbox   *accelerate;
	GLUI_Spinner    *deltat;
	GLUI_Panel      *panel_load;
	GLUI_Panel      *panel_sim;
	GLUI_Panel      *panel_step;
	GLUI_Panel      *panel_inf;
	GLUI_Panel      *panel_bonus;
	GLUI_Panel      *panel_edit;
}



/*--------------------------------------------------------------------*/
#define LOAD 		1
#define DELTA 		2
#define START 		3
#define PAUSE 		4
#define STEP		5
#define STEP_GO		6
#define OBST		7
#define PART		8



/*--------------------------------------------------------------------*/
void reshape(int x, int y);
void display(void);
void control_cb(int ID);
void idle(void);




/*--------------------------------------------------------------------*/
void reshape(int x, int y)
{
	glViewport(0, 0, x, y);

	aspect_ratio = (float) x / y ;
}



/*--------------------------------------------------------------------*/
void display(void)
{ 
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	
	
//l'aspect ratio est modifié dans display pour 
//pouvoir gerer un glOrtho qui change constamment a cause du zoom
	if(sim)
	{
		//necessaire pour le glOrtho sans zoom
		float r = sim_monde_get_rayon(sim);
		
		
		if(zoomed)
		{
			float xmin = (sim_robot_get_pos(sim).px - R_ROBOT * 3);
			float xmax = (sim_robot_get_pos(sim).px + R_ROBOT * 3);
			float ymin = (sim_robot_get_pos(sim).py - R_ROBOT * 3);
			float ymax = (sim_robot_get_pos(sim).py + R_ROBOT * 3);
			
			if(aspect_ratio <= 1.)
			{
				x_min = xmin; x_max = xmax;
				
				y_min = sim_robot_get_pos(sim).py - 
						R_ROBOT * 3/aspect_ratio;
				
				y_max = sim_robot_get_pos(sim).py + 
						R_ROBOT * 3/aspect_ratio;
			}
			else
			{
				x_min = sim_robot_get_pos(sim).px - 
						R_ROBOT * 3*aspect_ratio;
				
				x_max = sim_robot_get_pos(sim).px + 
						R_ROBOT * 3*aspect_ratio;
				
				y_min = ymin ; y_max = ymax ;
			}
		}
		else
		{
				x_min = -r; x_max = r;
				y_min = -r; y_max = r;
				
				if(aspect_ratio <= 1.)
				{
					x_min = -r; x_max = r;
					y_min = -r/aspect_ratio ;
					y_max =  r/aspect_ratio ;
				}
				else
				{
					x_min = -r*aspect_ratio;
					x_max =  r*aspect_ratio;
					y_min = -r ; y_max = r ;
				}
		}
	}
	
	
	glOrtho(x_min, x_max, y_min, y_max, -1, 1);
	
	if(sim)
		sim_draw(sim);
	
	glutSwapBuffers();
}



/*--------------------------------------------------------------------*/
void control_cb(int ID)
{
	switch (ID)
	{
	case LOAD: 
				
				simulate = 0;

				if(sim)
					sim_destroy(sim);

				sim = sim_create();
				
				if((sim = sim_get(filename->get_text())))
				{
					sim_surface_create(sim);
					elapsed->set_float_val( sim_get_temps(sim) );
					collision->set_int_val( sim_get_nb_collis(sim) );
					cleaned->set_int_val(0);
					total->set_int_val( sim_monde_get_nb_part(sim) );
					status->set_text((char*) "Load OK");
					surface->set_text((char*) "0.000 %");
					
					deltat->set_float_val( sim_get_deltat(sim) );
					
					panel_sim->enable();
					panel_edit->enable();
					step->disable();
					stepb->set_int_val(0);
					accelerate->set_int_val(0);
					zoom->set_int_val(0);
					part->set_int_val(0);
					obst->set_int_val(0);
					
					glutPostRedisplay();
				}
				else
					status->set_text( (char*) "Load FAIL");
					
				break;
				
	case DELTA: 
				
				sim_set_deltat(sim, deltat->get_float_val());
				break;
	
	
	case START:
				
				simulate = 1;
				panel_sim->disable();
				panel_edit->disable();
				paus->enable();
				deltat->disable();
				status->set_text( (char*) "Simulating");
				break;
				
	
	case PAUSE:
				
				simulate = 0;
				deltat->enable();
				start->enable();
				paus->disable();
				panel_step->enable();
				step->disable();
				status->set_text( (char*) "Paused");
				break;
				
	case STEP:
				
				if(stepmode)
				{
					panel_sim->disable();
					stepb->enable();
					step->enable();
				}
				else
				{
					panel_sim->disable();
					start->enable();
					deltat->enable();
					stepb->enable();
				}
				break;
				
	case STEP_GO:
				
				if(sim)
				{
					sim_update(sim);
					
					elapsed->set_float_val(sim_get_temps(sim));
					collision->set_int_val(sim_get_nb_collis(sim));
					cleaned->set_int_val((sim_monde_get_nb_part(sim) - 
										  sim_get_nb_part_actuel(sim)));
										  
										
					if(sim_get_temps(sim) >= MAX_DURATION || 
					   sim_get_nb_part_actuel(sim) <= 0 )
					{
						panel_sim->disable();
						panel_load->enable();
						status->set_text( (char*) "Finished");
					}
					
					glutPostRedisplay();
				}
				
				break;
				
	case OBST :
						
	case PART :
				
				if(ID == OBST)
					sim_create_obst(sim, obst->get_int_val());
				else
					sim_create_part(sim, part->get_int_val());
				
				cleaned->set_int_val(0);
				total->set_int_val( sim_monde_get_nb_part(sim) );
					
				panel_load->enable();
				panel_sim->enable();
				paus->disable();	
				panel_step->enable();
				
				glutPostRedisplay();
				
				break;		
	}
}



/*--------------------------------------------------------------------*/
void idle(void)
{
	if( glutGetWindow() != main_window)
		glutSetWindow(main_window);
	
	if(simulate)
	{
		if(sim)
		{
			sim_update(sim);
			
			//met à jour GLUI seulement si accelerate n'est pas coché
			//et sinon si la simulation est terminée
			if(!accelerated || sim_get_temps(sim) >= MAX_DURATION || 
			   sim_get_nb_part_actuel(sim) <= 0 )
			{
				elapsed->set_float_val(sim_get_temps(sim));
				collision->set_int_val(sim_get_nb_collis(sim));
				cleaned->set_int_val((sim_monde_get_nb_part(sim) - 
									  sim_get_nb_part_actuel(sim)));
				char tab[10];
				sprintf(tab, "%.3f %%", (float) 
										sim_surface_get(sim)/1000);
										
				surface->set_text( tab );
			}
								  
								
			if(sim_get_temps(sim) >= MAX_DURATION)
			{
				simulate = 0;
				
				panel_sim->disable();
				panel_load->enable();
				status->set_text( (char*) "Clean FAIL");
			}
			
			if(sim_get_nb_part_actuel(sim) <= 0)
			{
				simulate = 0;
				
				panel_sim->disable();
				panel_load->enable();
				status->set_text( (char*) "SUCCESS");
			}
			
			//affiche l'image seulement si accelerate n'est pas coché
			//et sinon si la simulation est terminée
			if(!accelerated || sim_get_temps(sim) >= MAX_DURATION || 
			   sim_get_nb_part_actuel(sim) <= 0 )
				glutPostRedisplay();
		}
	}
}

/*--------------------------------------------------------------------*/
/*							   MAIN									  */
/*--------------------------------------------------------------------*/
int main(int  argc, char * argv[])
{
	simulate = 0;
	sim = NULL;
	srand(time(NULL));
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize( 600, 650 );

	main_window = glutCreateWindow( "Aspirator" );	
	GLUI *glui = GLUI_Master.create_glui((char*) "GLUI", 0, 400, 50 );
	glui->set_main_gfx_window( main_window );
	
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	GLUI_Master.set_glutIdleFunc( idle );
	


	
	
/*--------------------------------------------------------------------*/
	panel_load = glui->add_panel( (char*) "Load Simulation" );
	filename = glui->add_edittext_to_panel(panel_load,(char*)"Filename", 
										   GLUI_EDITTEXT_TEXT, NULL, 
										   LOAD, control_cb);
	load = glui->add_button_to_panel(panel_load, (char*) "Load/Reset", 
									 LOAD, control_cb);
	
	
	
	
	panel_sim = glui->add_panel( (char*) "Simulation" );
	start = glui->add_button_to_panel(panel_sim, (char*) "Start", 
									  START, control_cb);
	paus = glui->add_button_to_panel(panel_sim, (char*) "Pause", 
									  PAUSE, control_cb);
	deltat = glui->add_spinner_to_panel(panel_sim, (char*)"Delta t", 
										GLUI_SPINNER_FLOAT, NULL, 
										DELTA, control_cb);
		
		
		
		
	panel_step = glui->add_panel_to_panel(panel_sim, 
										  (char*) "Step mode" );
	stepb = glui->add_checkbox_to_panel(panel_step,(char*)"Step by Step" 
										, &stepmode, STEP, control_cb);
	step = glui->add_button_to_panel(panel_step, (char*)"Step", 
									 STEP_GO, control_cb);
	
	
	
	
	panel_inf = glui->add_panel( (char*) "Information" );
	elapsed = glui->add_edittext_to_panel(panel_inf, (char*)"Time", 
										  GLUI_EDITTEXT_FLOAT);
	collision = glui->add_edittext_to_panel(panel_inf, 
											(char*)"Collisions");
	cleaned = glui->add_edittext_to_panel(panel_inf, 
										 (char*)"Particules cleaned");
	total = glui->add_edittext_to_panel(panel_inf, 
										(char*)"Total particules");
	status = glui->add_edittext_to_panel(panel_inf, (char*)"Status");
	
	
	
	
	panel_bonus = glui->add_panel( (char*) "Bonus" );
	zoom = glui->add_checkbox_to_panel(panel_bonus, (char*) "Zoom", 
									   &zoomed );
	accelerate = glui->add_checkbox_to_panel(panel_bonus, 
											 (char*) "Accelerate", 
											 &accelerated);
	surface = glui->add_edittext_to_panel(panel_bonus, 
										  (char*)"Surface :");
	panel_edit = glui->add_panel_to_panel(panel_bonus, 
										  (char*) "Edit World" );
	part = glui->add_edittext_to_panel(panel_edit,(char*)"Particules :", 
									   GLUI_EDITTEXT_INT, NULL, 
									   PART, control_cb);
	obst = glui->add_edittext_to_panel(panel_edit, (char*)"Obstacles :", 
									   GLUI_EDITTEXT_INT, NULL, 
									   OBST, control_cb);
	
	
	
	
	quit = glui->add_button((char*) "Quit", 0,(GLUI_Update_CB)exit);
	
	
	
/*--------------------------------------------------------------------*/	
	filename->set_text((char*) "");
	panel_sim->disable();
	status->set_text( (char*) "Waiting" );
	
	part->set_int_limits(0 , 1000000, GLUI_LIMIT_CLAMP);
	obst->set_int_limits(0 , 10, GLUI_LIMIT_CLAMP);
	deltat->set_float_limits(0.001 , 1, GLUI_LIMIT_CLAMP);
	
	
	
	
	if(argv[1])
	{
		filename->set_text(argv[1]);
		control_cb(LOAD);
		panel_sim->enable();
	}
		
	

	
	glutMainLoop();
   
	return EXIT_SUCCESS;
}
