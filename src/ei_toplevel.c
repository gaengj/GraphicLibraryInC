#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ei_button_intern.h"

void draw_toplevel(ei_surface_t	surface,
                  ei_rect_t* rectangle,
                  const ei_point_t*	where,
                  const char*		text,
                  const ei_font_t	font,
                  const ei_color_t color_font,
                  const ei_color_t color_frame,
                  int border_width,
                  const ei_rect_t*	clipper)
{
        /* A function that draws a toplevel
         */

        /* We draw the outer frame */
        rounded_frame(surface, rectangle, 0.0, EI_TRUE, EI_TRUE, EI_TRUE, color_frame, clipper);

        //int THICKNESS = 3;
        int HEADBAND_HEIGHT = 30;
        int SIZE_REDIM = 15;

        /* We draw the inner frame */
        ei_rect_t* interior_rectangle = malloc(sizeof(ei_rect_t));
        ei_color_t color_interior_rectangle = { 255, 255, 255, 255};
        interior_rectangle->top_left.x = rectangle->top_left.x + border_width;
        interior_rectangle->top_left.y = rectangle->top_left.y + HEADBAND_HEIGHT;
        interior_rectangle->size.width = rectangle->size.width - 2*border_width;
        interior_rectangle->size.height = rectangle->size.height - border_width - HEADBAND_HEIGHT;

        rounded_frame(surface, interior_rectangle, 0.0, EI_TRUE, EI_TRUE, EI_TRUE, color_interior_rectangle, interior_rectangle);
        free(interior_rectangle);

        /* We draw the close button */
        ei_rect_t* rectangle_button = malloc(sizeof(ei_rect_t));
        ei_color_t color_button		= { 255, 0, 0, 255};

      	ei_point_t point_top_left_button;
        point_top_left_button.x = rectangle->top_left.x + border_width;
        point_top_left_button.y = rectangle->top_left.y + border_width;

      	rectangle_button->top_left = point_top_left_button;
      	ei_size_t size_button;
      	size_button.width = 20;
      	size_button.height = 20;
      	rectangle_button->size = size_button;

        draw_button(surface, rectangle_button, 3.0, 4, EI_TRUE, EI_TRUE, &point_top_left_button, NULL, NULL, color_frame, color_button, clipper);
        free(rectangle_button);

        /* We draw the text */
        ei_point_t point_top_left_text;
        point_top_left_text.x = point_top_left_button.x + 20 + border_width;
        point_top_left_text.y = point_top_left_button.y;
        ei_draw_text(surface, &point_top_left_text, text, font, &color_font, rectangle);

        /* We draw the resize button */
        ei_rect_t* redim_rectangle = malloc(sizeof(ei_rect_t));
        redim_rectangle->top_left.x = rectangle->top_left.x + rectangle->size.width - SIZE_REDIM;
        redim_rectangle->top_left.y = rectangle->top_left.y + rectangle->size.height - SIZE_REDIM;
        redim_rectangle->size.width = SIZE_REDIM;
        redim_rectangle->size.height = SIZE_REDIM;

        rounded_frame(surface, redim_rectangle, 0.0, EI_TRUE, EI_TRUE, EI_TRUE, color_frame, clipper);
        free(redim_rectangle);
}
