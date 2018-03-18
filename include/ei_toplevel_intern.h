#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* A function that draws a toplevel
 */
void draw_toplevel(ei_surface_t	surface,
                  ei_rect_t* rectangle,
                  const ei_point_t*	where,
                  const char*		text,
                  const ei_font_t	font,
                  const ei_color_t color_font,
                  const ei_color_t color_frame,
                  int border_width,
                  const ei_rect_t*	clipper);
