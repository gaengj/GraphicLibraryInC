#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ei_button_intern.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

/* Global variables to free the allocations of arc */
static ei_linked_point_t *GLOB1;
static ei_linked_point_t *GLOB2;
static ei_linked_point_t *GLOB3;
static ei_linked_point_t *GLOB4;
static ei_linked_point_t *GLOB5;
static ei_linked_point_t *GLOB6;
static ei_linked_point_t *GLOB7;
static ei_linked_point_t *GLOB8;
static ei_linked_point_t *GLOB9;
static ei_linked_point_t *GLOB10;
static ei_linked_point_t *GLOB11;
static ei_linked_point_t *GLOB12;

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
        ({ __typeof__ (a) _a = (a); \
            __typeof__ (b) _b = (b); \
          _a < _b ? _a : _b; })

ei_color_t color_brighter(const ei_color_t color)
{
        /* Calculates a new color brighter than the color entered
         */

        ei_color_t color_bright = {0,0,0,0};
        int max = 0.0;
        if (color.red > max) {
                max = color.red;
        }
        if (color.green > max){
                max = color.green;
        }
        if (color.blue > max){
                max = color.blue;
        }

        color_bright.red = (int)(color.red*(255.0/max));
        color_bright.green = (int)(color.green*(255.0/max));
        color_bright.blue = (int)(color.blue*(255.0/max));
        color_bright.alpha = color.alpha;

        return(color_bright);
}

ei_color_t color_darker(const ei_color_t color)
{
        /* Calculates a new color darker than the color entered
        */
        ei_color_t color_dark = {0,0,0,0};
        int min = 255;
        if (color.red < min) {
                min = color.red;
        }
        if (color.green < min){
                min = color.green;
        }
        if (color.blue < min){
                min = color.blue;
        }

        color_dark.red = (int)(color.red*(1.0/min));
        color_dark.green = (int)(color.green*(1.0/min));
        color_dark.blue = (int)(color.blue*(1.0/min));
        color_dark.alpha = color.alpha;

        return(color_dark);
}

ei_linked_point_t *AllocateMemory (void)
{
        /* Allocates the memory for a ei_linked_point_t
         */

        return (ei_linked_point_t *)malloc(sizeof (ei_linked_point_t));
}

ei_linked_point_t* arc(ei_point_t center, double radius, double angle1, double angle2)
{
        /* Draws an arc of a given radius between two angles
         */

        int nb_segments_on_arc=10;
        double angle3 = (angle2 - angle1) / (nb_segments_on_arc -1);
        ei_linked_point_t* liste_points = calloc(nb_segments_on_arc, sizeof(ei_linked_point_t));
        ei_linked_point_t current;

        /* Construction of the list of points of the arc */
        current.point.x = radius * cos (angle1) + center.x;
        current.point.y = radius * sin (angle1) + center.y;
        current.next = NULL;
        liste_points[0] = current;

        for (uint32_t i = 1; i < nb_segments_on_arc; i++){
              current.point.x = radius * cos (angle1+(double)i*angle3) + center.x;
              current.point.y = radius * sin (angle1+(double)i*angle3) + center.y;
              liste_points[i] = current;
              liste_points[i-1].next = &(liste_points[i]);
        }
        liste_points[nb_segments_on_arc-1].next = NULL;

        return liste_points;
}

void rounded_frame(ei_surface_t	surface, ei_rect_t* rectangle, double radius,
                      ei_bool_t high_part, ei_bool_t low_part, ei_bool_t same_color,
                       const ei_color_t	color, const ei_rect_t*	clipper)
{
        /* Draws the two parts of a rounded frame
         */

        ei_linked_point_t* list_point_button;
        ei_linked_point_t* list_point_button2;

        /* Calculations of the colors for the relief effect */
        ei_color_t color_high;
        ei_color_t color_low;

        if (same_color == EI_TRUE){
                color_high = color;
                color_low = color;
        } else {
                 color_high = color_darker(color);
                 color_low = color_brighter(color);
        }

        /* We add successively the lists of points to the list_point_button */
        if (high_part == EI_TRUE && low_part == EI_TRUE){
                list_point_button = rounded_frame_high(rectangle, radius, color_high);
                ei_draw_polygon(surface, list_point_button, color_high, clipper);

                free(GLOB1);
                free(GLOB2);
                free(GLOB3);
                free(GLOB4);
                free(GLOB5);
                free(GLOB6);

                list_point_button2 = rounded_frame_low(rectangle, radius, color_low);
                ei_draw_polygon(surface, list_point_button2, color_low, clipper);

                free(GLOB7);
                free(GLOB8);
                free(GLOB9);
                free(GLOB10);
                free(GLOB11);
                free(GLOB12);

        } else {
                if (high_part == EI_TRUE) {
                        list_point_button = rounded_frame_high(rectangle, radius, color_high);
                        ei_draw_polygon(surface, list_point_button, color_high, clipper);

                        free(GLOB1);
                        free(GLOB2);
                        free(GLOB3);
                        free(GLOB4);
                        free(GLOB5);
                        free(GLOB6);

                } else {
                        list_point_button = rounded_frame_low(rectangle, radius, color_low);
                        ei_draw_polygon(surface, list_point_button, color_low, clipper);

                        free(GLOB7);
                        free(GLOB8);
                        free(GLOB9);
                        free(GLOB10);
                        free(GLOB11);
                        free(GLOB12);
                        }
                }
}

ei_linked_point_t* rounded_frame_high(ei_rect_t* rectangle, double radius, const ei_color_t	color)
{
        /* Draws the upper part of the rounded frame
         */

        /* Calculation of the coordinates of the points corresponding to the
         * corners of the rectangle */
        ei_point_t coin_ne, coin_sw;
        ei_point_t coin_nw = rectangle->top_left;

        coin_ne.x = (rectangle->size.width) + coin_nw.x;
        coin_ne.y = coin_nw.y;
        coin_sw.x = coin_nw.x;
        coin_sw.y = (rectangle->size.height) + coin_nw.y;

        /* Calculation of the coordinates of the points corresponding to the
         * centers of rounded arches */
        ei_point_t center_nw, center_sw, center_ne;

        center_nw.x = coin_nw.x + radius;
        center_nw.y = coin_nw.y + radius;

        center_ne.x = coin_ne.x - radius;
        center_ne.y = coin_ne.y + radius;

        center_sw.x = coin_sw.x + radius;
        center_sw.y = coin_sw.y - radius;

        /* Calculation of the coordinates of the 2 points inside the rectangle
         * (cf poly scheme) and of the parameter h */
        ei_point_t middle_left, middle_right;

        int h = min((rectangle->size.width)/2, (rectangle->size.height)/2);

        middle_right.x = coin_ne.x - h;
        middle_right.y = coin_ne.y + h;

        middle_left.x = coin_sw.x + h;
        middle_left.y = coin_sw.y - h;

        /* Creation of the two cells corresponding to these last two points */
        ei_linked_point_t* cellule_middle_right = NULL;
        ei_linked_point_t* cellule_middle_left = NULL;
        cellule_middle_right = AllocateMemory();
        GLOB1 = cellule_middle_right;
        cellule_middle_right->point = middle_right;
        cellule_middle_right->next = NULL;

        cellule_middle_left = AllocateMemory();
        GLOB2 = cellule_middle_left;
        cellule_middle_left->point = middle_left;
        cellule_middle_left->next = NULL;

        /* Initialization of the chained list of points and two pointers on the
         * chained list: a pointer to the last non-NULL cell and a pointer to
         * the next cell to this cell */
        ei_linked_point_t* liste_point_high;
        ei_linked_point_t* next_point;
        ei_linked_point_t* last_no_NULL;
        ei_linked_point_t* addr_first_point = AllocateMemory();
        GLOB3 = addr_first_point;

        /* Adding of the different sets of points to the linked list */
        liste_point_high = arc(center_nw, radius, -M_PI, -0.5*M_PI);
        GLOB4 = liste_point_high;
        *addr_first_point = liste_point_high[0];
        addr_first_point->next = NULL;

        next_point = liste_point_high;
        while (next_point != NULL){
              last_no_NULL = next_point;
              next_point = next_point->next;
        }
        last_no_NULL->next = arc(center_ne, radius, -0.5*M_PI, -0.25*M_PI);
        GLOB5 = last_no_NULL->next;
        next_point = last_no_NULL;
        while (next_point != NULL){
              last_no_NULL = next_point;
              next_point = next_point->next;
        }

        last_no_NULL->next = cellule_middle_right;

        last_no_NULL = last_no_NULL->next;
        last_no_NULL->next = cellule_middle_left;
        last_no_NULL = last_no_NULL->next;

        last_no_NULL->next = arc(center_sw, radius, 0.75*M_PI, M_PI);
        GLOB6 = last_no_NULL->next;

        next_point = last_no_NULL;
        while (next_point != NULL){
              last_no_NULL = next_point;
              next_point = next_point->next;
        }

        last_no_NULL->next = addr_first_point;

        return liste_point_high;
}

ei_linked_point_t* rounded_frame_low(ei_rect_t* rectangle, double radius, const ei_color_t	color)
{
        /* Draws the lower part of the rounded frame
         */

         /* Calculation of the coordinates of the points corresponding to the
          * corners of the rectangle */
        ei_point_t coin_ne, coin_sw, coin_se;

        ei_point_t coin_nw = rectangle->top_left;
        coin_ne.x = (rectangle->size.width) + coin_nw.x;
        coin_ne.y = coin_nw.y;
        coin_sw.x = coin_nw.x;
        coin_sw.y = (rectangle->size.height) + coin_nw.y;
        coin_se.x = (rectangle->size.width) + coin_nw.x;
        coin_se.y = (rectangle->size.height) + coin_nw.y;

        /* Calculation of the coordinates of the points corresponding to the
         * centers of rounded arches */
        ei_point_t center_sw, center_se, center_ne;

        center_ne.x = coin_ne.x - radius;
        center_ne.y = coin_ne.y + radius;

        center_se.x = coin_se.x - radius;
        center_se.y = coin_se.y - radius;

        center_sw.x = coin_sw.x + radius;
        center_sw.y = coin_sw.y - radius;

        /* Calculation of the coordinates of the 2 points inside the rectangle
         * (cf poly scheme) and of the parameter h */
        ei_point_t middle_left, middle_right;

        int h = min((rectangle->size.width)/2, (rectangle->size.height)/2);

        middle_right.x = coin_ne.x - h;
        middle_right.y = coin_ne.y + h;

        middle_left.x = coin_sw.x + h;
        middle_left.y = coin_sw.y - h;

        /* Creation of the two cells corresponding to these last two points */
        ei_linked_point_t* cellule_middle_right;
        ei_linked_point_t* cellule_middle_left;

        cellule_middle_right = AllocateMemory();
        GLOB7 = cellule_middle_right;
        cellule_middle_right->point = middle_right;
        cellule_middle_right->next = NULL;

        cellule_middle_left = AllocateMemory();
        GLOB8 = cellule_middle_left;
        cellule_middle_left->point = middle_left;
        cellule_middle_left->next = NULL;

        /* Initialization of the chained list of points and two pointers on the
         * chained list: a pointer to the last non-NULL cell and a pointer to
         * the next cell to this cell */
        ei_linked_point_t* liste_point_low;
        ei_linked_point_t* next_point;
        ei_linked_point_t* last_no_NULL;
        ei_linked_point_t* addr_first_point = AllocateMemory();
        GLOB9 = addr_first_point;

        /* Adding of the different sets of points to the linked list */
        liste_point_low = arc(center_ne, radius, -0.25*M_PI, 0);
        GLOB10 =liste_point_low;
        *addr_first_point = liste_point_low[0];
        addr_first_point->next = NULL;

        next_point = liste_point_low;
        while (next_point != NULL){
                last_no_NULL = next_point;
                next_point = next_point->next;
        }

        last_no_NULL->next = arc(center_se, radius, 0, 0.5*M_PI);
        GLOB11 = last_no_NULL->next;

        next_point = last_no_NULL;
        while (next_point != NULL){
                last_no_NULL = next_point;
                next_point = next_point->next;
        }

        last_no_NULL->next = arc(center_sw, radius, 0.5*M_PI, 0.75*M_PI);
        GLOB12 = last_no_NULL->next;

        next_point = last_no_NULL;
        while (next_point != NULL){
                last_no_NULL = next_point;
                next_point = next_point->next;
        }

        last_no_NULL->next = cellule_middle_left;
        last_no_NULL = last_no_NULL->next;
        last_no_NULL->next = cellule_middle_right;
        last_no_NULL = last_no_NULL->next;

        last_no_NULL->next = addr_first_point;

        return liste_point_low;
}

void draw_button (ei_surface_t	surface,
                  ei_rect_t* rectangle,
                  double radius,
                  int border_width,
                  ei_bool_t high_part,
                  ei_bool_t  low_part,
                  const ei_point_t*	where,
                  const char*		text,
                  const ei_font_t	font,
                  const ei_color_t	color_font,
                  const ei_color_t	color_button,
                  const ei_rect_t*	clipper)
{
      /* A function that draws a button
       */

      /* The external frame of the button is drawn, the one giving the
       * impression of relief */
      rounded_frame(surface, rectangle, radius, high_part, low_part, EI_FALSE, color_button, clipper);

      /* We draw the inner frame of the button */
      ei_rect_t* interior_rectangle = malloc(sizeof(ei_rect_t));

      interior_rectangle->top_left.x = rectangle->top_left.x + border_width;
      interior_rectangle->top_left.y = rectangle->top_left.y + border_width;
      interior_rectangle->size.width = rectangle->size.width - 2*border_width;
      interior_rectangle->size.height = rectangle->size.height - 2*border_width;

      rounded_frame(surface, interior_rectangle, radius, high_part, low_part, EI_TRUE, color_button, clipper);

      /* We draw the text inside the button: */
      int width_text = 0;
      int height_text = 0;
      if (text != NULL){
              hw_text_compute_size(text, font, &width_text, &height_text);

              /* Calculation of the position of the text in the button */
              ei_point_t* point_button = malloc(sizeof(ei_point_t));

              point_button->x = interior_rectangle->top_left.x + (interior_rectangle->size.width - width_text)/2;
              point_button->y = interior_rectangle->top_left.y + (interior_rectangle->size.height - height_text)/2;

              /* We draw the text */
              ei_draw_text(surface, point_button, text, font, &color_font, interior_rectangle);
              free(point_button);
      }
      free(interior_rectangle);
}
