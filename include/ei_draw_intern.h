#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include <stdio.h>
#include <stdlib.h>

/* Define a structure "polygon_side", which represents the side of a polygon */
struct polygon_side{
        int ordinate_max;
        double abcisse_ordinate_min;
        int error;
        int delta_x;
        int delta_y;
        struct polygon_side *next_side;
};

/* Converts the three red, green and blue component of a color in a 32
 * bits integer using the order of the channels of the surface.
 */
uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color);

/* A function which get the indices of the components red, green, blue
 * and alpha of surface
 */
ei_color_t ei_demap_rgba(ei_surface_t surface,uint32_t pixel);

/* Draw a line made of many lines segments
 */
void ei_draw_polyline(ei_surface_t surface,
              			  const ei_linked_point_t *first_point,
              			  const ei_color_t color,
              			  const ei_rect_t *clipper);

/* A function which draws a segment between two points
 */
void ei_draw_segment(ei_point_t first_point,
                     ei_point_t second_point,
                     ei_size_t size_surface,
                     uint32_t* pixel_ptr,
                     uint32_t	value_color,
                     const ei_rect_t*  clipper);

/* Fills the surface with the specified color.
*/
void ei_fill(ei_surface_t	surface, const ei_color_t *color,const ei_rect_t *clipper);

/* Draws a polygon on the surface
 */
void ei_draw_polygon(ei_surface_t surface,
                     const ei_linked_point_t *first_point,
                     const ei_color_t color,
                     const ei_rect_t *clipper);

/* Creates the table of sides
*/
struct polygon_side **ei_create_tc(const ei_linked_point_t *first_point, ei_size_t size_surface);

/* Creates a side of polygon
 */
struct polygon_side *ei_create_side_of_polygon(ei_point_t first_point, ei_point_t second_point);

void insert_side_in_tab(struct polygon_side **tab_tc, struct polygon_side *adress_side, int indice);

/* Finds the point of minimum ordinate and returns its value
 */
int ordonne_min(ei_point_t first_point, ei_point_t second_point);

/* Allows the creation of a tca array
 */
struct polygon_side **ei_create_tca();

/* Initializes the scanline
 */
int init_scanline(const ei_linked_point_t *first_point);

/* Verifies if an array is empty
 */
int is_tab_empty(struct polygon_side **tab_tc, int length_tab);

/* Fills a tca array
 */
void ei_fill_tca(struct polygon_side **tab_tca, struct polygon_side **tab_tc, int current_scanline);

/* Delete all the polygon_side whose field ordinate_max value scanline
*/
void ei_delete_old_side_tca(struct polygon_side **tab_tca, int current_scanline);

/* Sorts all the elements of a tca array by their abscissa
 */
void ei_sort_by_abcisse_tca(struct polygon_side **tab_tca);

/* Copies a struct polygon_side
 */
void ei_copy_polygon_side(struct polygon_side *current_pointer, struct polygon_side *adress_copy_side);

/* Inserts a polygon_side at the right place in a sorted list
 * of polygon_side
 */
void insert_side_in_sorted_list(struct polygon_side **tab, struct polygon_side *adress_side);

/* Fill the line of the ordinate of the scanline
 */
void ei_draw_pixels_on_scanline(uint32_t value_color,ei_size_t size_surface,
                                uint32_t *first_pixel_pointeur, const ei_rect_t *clipper,
                                struct polygon_side **tab_tca, int scanline);

/* Calculates the right abscissa in accordance with the convention
 * described in the poly
 */
int roudish_abcisse(double x, int entree);

double floor(double x);

/* Updates a tca array in accordance with the algorithm
 * described in the poly
 */
void ei_update_abcisse_tca(struct polygon_side **tab_tca, int current_scanline);

/* Draws text by calling hw_text_create_surface.
 */
void			ei_draw_text		(ei_surface_t		surface,
            const ei_point_t*	where,
            const char*		text,
            const ei_font_t	font,
            const ei_color_t*	color,
            const ei_rect_t*	clipper);

/* Calculates the new value of a pixel when transparency is effective
 */
ei_color_t*    ei_transparency   (ei_color_t dst_color, ei_color_t src_color);

/* Copies a surface, or a subpart, to another one.
 */
int			  ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha);

/* --- Printing functions : used for debug only --- */

void printf_tab(struct polygon_side **tab, int length_tab);

void printf_point(ei_point_t point);
