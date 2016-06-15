/* Nom: graphic.c
 * Description: Module graphic, s'occupe du dessin open_GL
 * Date: 21.05.2011
 * Auteur: -
 */


/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#include "graphic.h"

void graphic_draw_segment (float x1,
                           float y1,
                           float x2,
                           float y2)

{ 
    glBegin (GL_LINES);

    glVertex2f (x1, y1);
    glVertex2f (x2, y2);

    glEnd ();
}

/*--------------------------------------------------------------------*/

void graphic_draw_rectangle (float xc,
			     float yc,
			     float width,
                             float height,
                             int   filled)

{   if (filled == GRAPHIC_FILLED)
      glBegin (GL_POLYGON);
    else
      glBegin (GL_LINE_LOOP);

    glVertex2f (xc+width/2, yc+height/2);
    glVertex2f (xc-width/2, yc+height/2);
    glVertex2f (xc-width/2, yc-height/2);
    glVertex2f (xc+width/2, yc-height/2);

    glEnd ();
}
/*--------------------------------------------------------------------*/

void graphic_draw_circle (float xc,
                          float yc,
                          float r,
                          int   filled)

{ int i;
  const int SIDES = 50;

  if (filled == GRAPHIC_FILLED)
    glBegin (GL_POLYGON);
  else
    glBegin (GL_LINE_LOOP);
  
  for (i=0; i < SIDES; i++)
    {
      float alpha = i * 2. * M_PI / SIDES;
        glVertex2f (xc+r*cos(alpha), yc+r*sin(alpha));
    }

  glEnd ();
}

/*--------------------------------------------------------------------*/

void graphic_set_color3f(float r,
                       float g,
                       float b)
{
  glColor3f(r, g, b);
}

void graphic_set_color3fv(float color[3])
{
  glColor3fv(color);
}

void graphic_set_line_width(float width)
{
  glLineWidth(width);
}
