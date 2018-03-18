#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"

/* Calculates a new color darker than the color entered
*/
ei_color_t color_darker(ei_color_t color);

/* Calculates a new color brighter than the color entered
 */
ei_color_t color_brighter(ei_color_t color);

/* Draws an arc of a given radius between two angles
 */
ei_linked_point_t* arc(ei_point_t center, double radius, double angle1, double angle2);

/* Draws the two parts of a rounded frame
 */
void rounded_frame(ei_surface_t	surface, ei_rect_t* rectangle, double radius,
                    ei_bool_t high_part, ei_bool_t low_part, ei_bool_t same_color,
                    const ei_color_t	color, const ei_rect_t*	clipper);

/* Draws the lower part of the rounded frame
 */
ei_linked_point_t* rounded_frame_low(ei_rect_t* rectangle, double radius, const ei_color_t	color);

/* Draws the upper part of the rounded frame
 */
ei_linked_point_t* rounded_frame_high(ei_rect_t* rectangle, double radius, const ei_color_t	color);

/* Allocates the memory for a ei_linked_point_t
 */
ei_linked_point_t *AllocateMemory (void);

/* A function that draws a button
 */
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
                  const ei_rect_t*	clipper);

#endif
