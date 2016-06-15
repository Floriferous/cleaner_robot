/* Nom: graphic.h
 * Description: Module graphic, s'occupe du dessin open_GL
 * Date: 21.05.2011
 * Auteur: -
 */



#ifndef GRAPHIC_H
#define GRAPHIC_H

#define GRAPHIC_EMPTY  0
#define GRAPHIC_FILLED 1



void       graphic_draw_segment (float x1,
                                 float y1,
                                 float x2, 
                                 float y2);

void       graphic_draw_rectangle ( float xc,
									float yc, 
									float width,
                                   float height,
                                   int   filled);

void       graphic_draw_circle (float xc,
                                float yc,
			        float r,
                                int   filled);

void       graphic_set_color3f(float r,
                             float g, 
                             float b);

void       graphic_set_color3fv(float color[3]);


void       graphic_set_line_width(float width);

#endif
