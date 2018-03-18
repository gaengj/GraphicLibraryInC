#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_draw_intern.h"
#include "ei_widgetclass_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ei_application_intern.h"

void ei_draw_polyline(ei_surface_t surface,
              			  const ei_linked_point_t *first_point,
              			  const ei_color_t color,
              			  const ei_rect_t *clipper)
{
        /* Draw a line made of many lines segments
         */

        hw_surface_lock(surface);

        ei_size_t size_surface = hw_surface_get_size(surface);
        uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        uint32_t value_color = ei_map_rgba(surface, &color);
        ei_linked_point_t current_point = *first_point;
        ei_linked_point_t next_point = *(current_point.next);

        /* We go through the pairs of points contained in the list chain.
         * The current pair is (current_point, next_point) */
        while (next_point.next != NULL){
                ei_draw_segment(current_point.point, next_point.point,
                   size_surface, pixel_ptr, value_color, clipper);
                current_point = next_point;
                next_point = *(next_point.next);
        }

        /* We draw the segement [current_point, next_point] */
        ei_draw_segment(current_point.point, next_point.point,
                        size_surface, pixel_ptr, value_color, clipper);
        current_point = next_point;
        next_point = *first_point;

        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, NULL);
}

uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color)
{
        /* Converts the three red, green and blue component of a color in a 32
         * bits integer using the order of the channels of the surface.
         */

        int id_red, id_green, id_blue, id_alpha;
        hw_surface_get_channel_indices(surface, &id_red, &id_green, &id_blue, &id_alpha);
        uint32_t value_color = (((uint32_t) color->red)<<(8*id_red)) +
                                  (((uint32_t) color->green)<<(8*id_green)) +
                                  (((uint32_t) color->blue)<<(8*id_blue)) +
                                  (((uint32_t) color->alpha)<<(8*id_alpha));
        return value_color;
}

ei_color_t ei_demap_rgba(ei_surface_t surface, uint32_t pixel)
{
        /* A function which get the indices of the components red, green, blue
         * and alpha of surface
         */

        int id_red, id_green, id_blue, id_alpha;
        ei_color_t color;
        hw_surface_get_channel_indices(surface, &id_red, &id_green, &id_blue, &id_alpha);
        color.red = pixel>>(8*id_red);
        color.blue = pixel>>(8*id_blue);
        color.green = pixel>>(8*id_green);
        color.alpha = pixel>>(8*id_alpha);
        return color;
}

void ei_draw_segment(ei_point_t first_point,
                     ei_point_t second_point,
                     ei_size_t size_surface,
                     uint32_t* pixel_ptr,
                     uint32_t	value_color,
                     const ei_rect_t*  clipper)
{
        /* A function which draws a segment between two points
         */

        /* Bruteforce implementation of the clipper : initialisation */
        uint32_t xmin, xmax, ymin, ymax;
        if (clipper != NULL){
                xmin = clipper->top_left.x;
                ymin = clipper->top_left.y;
                xmax = xmin + (clipper->size.width);
                ymax = ymin + (clipper->size.height);
        }

        /* Calculations of the different quantities */
        int32_t delta_x = second_point.x - first_point.x;
        int32_t delta_y = second_point.y - first_point.y;
        int sign_x = delta_x > 0 ? 1 : -1 ;
        int sign_y = delta_y > 0 ? 1 : -1 ;
        delta_x = abs(delta_x);
        delta_y = abs(delta_y);
        int E = 0;
        int x = first_point.x;
        int y = first_point.y;

        /* Implementation of the algorithm described in the poly */
        if (delta_x > delta_y){
                for (int i = 0; i < delta_x + 1; i++){
                        if (2*E > delta_x){
                                y = y + sign_y;
                                E = E - delta_x;
                        }

                        /* We draw the pixel of coordinates (x,y) */
                        if (clipper != NULL){
                                if ((x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax)){
                                      *(pixel_ptr +x+(size_surface.width)*y) = value_color;
                                }
                        }
                        else{
                                *(pixel_ptr +x+(size_surface.width)*y) = value_color;
                        }
                        x = x + sign_x;
                        E = E + delta_y;
                }
        }
        else {
                for (int i = 0; i < delta_y + 1; i++){
                        if (2*E > delta_y){
                                x = x + sign_x;
                                E = E - delta_y;
                        }

                        /* We draw the pixel of coordinates (x,y) */
                        if (clipper != NULL){
                                if ((x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax)){
                                      *(pixel_ptr +x+(size_surface.width)*y) = value_color;
                                }
                        }
                        else {
                                *(pixel_ptr +x+(size_surface.width)*y) = value_color;
                        }
                        y = y + sign_y;
                        E = E + delta_x;
                }
        }
}

void ei_fill(ei_surface_t	surface, const ei_color_t *color,const ei_rect_t *clipper)
{
        /* Fills the surface with the specified color.
         */

        hw_surface_lock(surface);

        /* Taking into account the clipper */
        uint32_t xmin, xmax, ymin, ymax;
        if (clipper != NULL){
                xmin = clipper->top_left.x;
                ymin = clipper->top_left.y;
                xmax = xmin + (clipper->size.width);
                ymax = ymin + (clipper->size.height);
        }
        ei_size_t size_surface = hw_surface_get_size(surface);
        uint32_t *pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        uint32_t pixel_color = ei_map_rgba(surface, color);

        if (clipper != NULL){
                for (int y = 0; y < size_surface.height; y++){
                      for (int x = 0; x < size_surface.width; x++){
                              if ((x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax)){
                                      pixel_ptr[x + size_surface.width*y] = pixel_color;
                              }
                      }
                }
        }
        else {
                for (int i = 0; i < size_surface.width * size_surface.height; i++){
                        *pixel_ptr++ = pixel_color;
                }
        }
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, NULL);
}

void ei_draw_polygon(ei_surface_t surface,
                     const ei_linked_point_t *first_point,
                     const ei_color_t color,
                     const ei_rect_t *clipper)
{
        /* Draws a polygon on the surface
         */

        /* Initialization */
        hw_surface_lock(surface);
        ei_size_t size_surface = hw_surface_get_size(surface);
        uint32_t* first_pixel_pointer = (uint32_t*)hw_surface_get_buffer(surface);
        uint32_t value_color = ei_map_rgba(surface, &color);
        struct polygon_side **tab_tc = ei_create_tc(first_point, size_surface);
        struct polygon_side **tab_tca = ei_create_tca();
        int current_scanline = init_scanline(first_point);

        /* Implementation of the algorithm described in the poly */
        while ( (current_scanline < size_surface.height) && ((is_tab_empty(tab_tc, size_surface.height) != 1) || (is_tab_empty(tab_tca, 1) != 1 ))){
                ei_fill_tca(tab_tca, tab_tc, current_scanline);
                ei_delete_old_side_tca(tab_tca, current_scanline);
                ei_sort_by_abcisse_tca(tab_tca);
                ei_draw_pixels_on_scanline(value_color, size_surface, first_pixel_pointer, clipper, tab_tca, current_scanline);
                current_scanline++;
                ei_update_abcisse_tca(tab_tca, current_scanline);

        }

        /* Free the old tab_tca */
        struct polygon_side *curr_ptr = tab_tca[0];
        struct polygon_side *next_ptr = NULL;
        while (curr_ptr != NULL){
                next_ptr = curr_ptr->next_side;
                free(curr_ptr);
                curr_ptr = next_ptr;
        }
        free(tab_tc);
        free(tab_tca);
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, NULL);
}

struct polygon_side **ei_create_tc(const ei_linked_point_t *first_point, ei_size_t size_surface)
{
        /* Creates the table of sides
         */

        struct polygon_side **tab_tc = calloc(size_surface.height, sizeof(struct polygon_side*));
        for (int i = 0; i < size_surface.height; i++){
                tab_tc[i] = NULL;
        }

        /* We sort the sides */
        ei_linked_point_t current_point = *first_point;
        ei_linked_point_t next_point;
        if (first_point != NULL){
                while (current_point.next != NULL){
                        next_point = *(current_point.next);

                        if (current_point.point.y != next_point.point.y){
                                /*  We ignore the horizontal sides */
                                struct polygon_side *adress_new_side = ei_create_side_of_polygon(current_point.point, next_point.point);
                                int indice  = ordonne_min(current_point.point, next_point.point);
                                insert_side_in_tab(tab_tc, adress_new_side, indice);
                        }
                        current_point = next_point;
                }
        }
        return tab_tc;
}

struct polygon_side *ei_create_side_of_polygon(ei_point_t first_point, ei_point_t second_point)
{
        /* Creates a side of polygon
         */

        struct polygon_side *adress_side = malloc(sizeof(struct polygon_side));

        ei_point_t high_point, low_point;

        /* High point is the point with the lower ordinate */
        if (first_point.y < second_point.y){
                high_point = first_point;
                low_point = second_point;
        }
        else{
                high_point = second_point;
                low_point = first_point;
        }
        int delta_x = low_point.x - high_point.x;
        int delta_y = low_point.y - high_point.y;
        adress_side->ordinate_max = low_point.y;
        adress_side->abcisse_ordinate_min = high_point.x;
        adress_side->error = 0;
        adress_side->delta_x = delta_x;
        adress_side->delta_y = abs(delta_y);
        adress_side->next_side = NULL;
        return adress_side;
}

void insert_side_in_tab(struct polygon_side **tab_tc, struct polygon_side *adress_side, int indice)
{
        struct polygon_side *pointer = tab_tc[indice];
        if (pointer == NULL){
                tab_tc[indice]= adress_side;

        }
        else{
                while (pointer->next_side != NULL){
                        pointer = pointer->next_side;
                }

                /* We add the side at the end of the list of polygon_side of
                 * tab_tc[indice] */
                pointer->next_side = adress_side;
        }
}

int ordonne_min(ei_point_t first_point, ei_point_t second_point)
{
        /* Finds the point of minimum ordinate and returns its value
         */

        if (first_point.y > second_point.y){
                return second_point.y;
        }
        else{
                return first_point.y;
        }
}

struct polygon_side **ei_create_tca()
{
        /* Allows the creation of a tca array
         */

        struct polygon_side **tab_tca = calloc(1, sizeof(struct polygon_side*));
        tab_tca[0] = NULL;
        return tab_tca;
}

int init_scanline(const ei_linked_point_t *first_point)
{
        /* Initializes the scanline
         */

        if (first_point != NULL) {
                const ei_linked_point_t *current = first_point;
                int ord_min = current->point.y;
                while (current->next != NULL){
                        if(current->next->point.y < ord_min){
                                ord_min = current->next->point.y;
                        }
                        current = current->next;
                }
                return ord_min;
        }
        else{
                /* Maybe to change */
                return 0;
        }
}

int is_tab_empty(struct polygon_side **tab, int length_tab)
{
        /* Verifies if an array is empty
         */

        for (int i = 0; i< length_tab; i++){
                struct polygon_side *addr = tab[i];
                if (addr != NULL){
                        return 0;
                }
        }
        return 1;
}

void ei_fill_tca(struct polygon_side **tab_tca, struct polygon_side **tab_tc, int current_scanline)
{
        /* Fills a tca array
         */

        /* pointer on the chained list in tab_tc[current_scanline] to add to
         * tab_tca */
        struct polygon_side *pointer_tc = tab_tc[current_scanline];

        /* pointer on the begin of the chained list in tab_tca */
        struct polygon_side *pointer_tca = tab_tca[0];
        if (pointer_tca != NULL){
                while (pointer_tca->next_side != NULL){
                        pointer_tca = pointer_tca->next_side;
                }
                struct polygon_side *next= NULL;
                while (pointer_tc != NULL){

                      /* We copy the sides of tab_tc[current_scanline] and put
                       * them in tab_tca */
                      struct polygon_side *adress_copy_side = malloc(sizeof(struct polygon_side));
                      ei_copy_polygon_side(pointer_tc, adress_copy_side);
                      pointer_tca->next_side = adress_copy_side;
                      pointer_tca = pointer_tca->next_side;
                      next = pointer_tc->next_side;

                      /* We free the sides of tab_tc[current_scanline] */
                      free(pointer_tc);
                      pointer_tc = next;
                }
        }
        else{
                tab_tca[0] = pointer_tc;
        }
}

void ei_delete_old_side_tca(struct polygon_side **tab_tca, int current_scanline)
{
        /* Delete all the polygon_side whose field ordinate_max value scanline
        */

        struct polygon_side side_sent;
        side_sent.ordinate_max = -1;
        side_sent.next_side = tab_tca[0];
        struct polygon_side* prec;
        struct polygon_side* curr;
        prec = &side_sent;
        curr = prec->next_side;
        struct polygon_side* suiv;

        /* We check if the list is not empty */
        while (curr != NULL){
                while (curr != NULL && curr->ordinate_max != current_scanline){
                        prec = prec->next_side;
                        curr = curr->next_side;
                }
                if (curr != NULL){

                        /* First occurence found */
                        suiv = curr->next_side;
                        free(curr);
                        while(suiv != NULL && suiv->ordinate_max == current_scanline){

                                /* If several occurrences are following  */
                                curr = suiv;
                                suiv = suiv->next_side;
                                free(curr);
                        }

                        /* We make the link */
                        prec->next_side = suiv;
                        prec = prec->next_side;
                        if (prec == NULL){

                                /* Case : we are at the end of the list */
                                curr = NULL;
                        } else {

                                /* Case : we are not at the end of the list */
                                curr = prec->next_side;
                        }
                }
       }
       tab_tca[0] = side_sent.next_side;
}

void ei_sort_by_abcisse_tca(struct polygon_side **tab_tca)
{
        /* Sorts all the elements of a tca array by their abscissa
         */

        struct polygon_side **new_list = ei_create_tca();
        struct polygon_side *current_pointer = tab_tca[0];

        while (current_pointer != NULL){
                struct polygon_side *adress_copy_side = malloc(sizeof(struct polygon_side));
                ei_copy_polygon_side(current_pointer, adress_copy_side);
                insert_side_in_sorted_list(new_list, adress_copy_side);
                current_pointer = current_pointer->next_side;
        }

        /* Free the old tab_tca */
        struct polygon_side *curr_ptr = tab_tca[0];
        struct polygon_side *next_ptr = NULL;
        while (curr_ptr != NULL){
                next_ptr = curr_ptr->next_side;
                free(curr_ptr);
                curr_ptr = next_ptr;
        }
        tab_tca[0] = new_list[0];
        free(new_list);
}

void ei_copy_polygon_side(struct polygon_side *current_pointer, struct polygon_side *adress_copy_side)
{
        /* Copies a struct polygon_side
         */

        adress_copy_side->ordinate_max = current_pointer->ordinate_max;
        adress_copy_side->abcisse_ordinate_min = current_pointer->abcisse_ordinate_min;
        adress_copy_side->error = current_pointer->error;
        adress_copy_side->delta_x = current_pointer->delta_x;
        adress_copy_side->delta_y = current_pointer->delta_y;
        adress_copy_side->next_side = NULL;
}

void insert_side_in_sorted_list(struct polygon_side **tab, struct polygon_side *adress_side)
{
        /* Inserts a polygon_side at the right place in a sorted list
         * of polygon_side
         */

        struct polygon_side side_sent;
        side_sent.abcisse_ordinate_min = -1;
        side_sent.next_side = tab[0];
        struct polygon_side *prec = &side_sent;
        struct polygon_side *curr = prec->next_side;


        while (curr != NULL && curr->abcisse_ordinate_min < adress_side->abcisse_ordinate_min){
                prec = curr;
                curr = curr->next_side;
        }
        adress_side->next_side = prec->next_side;
        prec->next_side = adress_side;
        tab[0] = side_sent.next_side;
}

void ei_draw_pixels_on_scanline(uint32_t value_color,ei_size_t size_surface,
                                uint32_t *first_pixel_pointeur, const ei_rect_t *clipper,
                                struct polygon_side **tab_tca, int scanline)
{
        /* Fill the line of the ordinate of the scanline
         */

        struct polygon_side *ptr = tab_tca[0];

        /* Bruteforce implementation of the clipper : initialisation */
        uint32_t xmin, xmax, ymin, ymax;
        if (clipper != NULL){
               xmin = clipper->top_left.x;
               ymin = clipper->top_left.y;
               xmax = xmin + (clipper->size.width);
               ymax = ymin + (clipper->size.height);
             }

        while (ptr != NULL){
                float abcisse_debut = ptr->abcisse_ordinate_min;
                ptr = ptr->next_side;
                float abcisse_fin = ptr->abcisse_ordinate_min;
                ptr = ptr->next_side;
                int debut = roudish_abcisse(abcisse_debut, 1);
                int fin = roudish_abcisse(abcisse_fin, 0);
                int y = scanline;
                for (int x = debut; x < fin + 1 ; x++){
                        /*We draw the pixel of coordinates (x,y)*/
                        if (clipper != NULL){
                                if ((x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax)){
                                      *(first_pixel_pointeur +x+(size_surface.width)*y) = value_color;
                                }
                        }
                        else {
                                *(first_pixel_pointeur +x+(size_surface.width)*y) = value_color;
                        }
                }
        }
}

int roudish_abcisse(double x, int entree)
{
        /* Calculates the right abscissa in accordance with the convention
         * described in the poly
         */

        int value = 0;
        double y = floor(x);
        if (x == y){
                value = (int)y;
        } else{
                value = (int)y + 1;
        }
        if (entree == 1){
                return value;
        } else{
                return value - 1;
        }
}

void ei_update_abcisse_tca(struct polygon_side **tab_tca, int current_scanline)
{
        /* Updates a tca array in accordance with the algorithm
         * described in the poly
         */

        struct polygon_side *pointer = tab_tca[0];
        while (pointer != NULL){
                int signe_x = pointer->delta_x > 0 ? 1 : -1 ;
                pointer->error = pointer->error + abs(pointer->delta_x);
                while (2*pointer->error > pointer->delta_y){
                      pointer->abcisse_ordinate_min = pointer->abcisse_ordinate_min + signe_x;
                      pointer->error = pointer->error - pointer->delta_y;
                }
                pointer = pointer->next_side;
        }
}

void			ei_draw_text		(ei_surface_t		surface,
            const ei_point_t*	where,
            const char*		text,
            const ei_font_t	font,
            const ei_color_t*	color,
            const ei_rect_t*	clipper)
{
              /* Draws text by calling hw_text_create_surface.
               */

              hw_surface_lock(surface);

              /* Calculation of the place taken by the text */
              int width = 0;
              int height = 0;
              hw_text_compute_size(text, font, &width, &height);

              /* Calculating the destination rectangle */
              ei_rect_t* dest_rect = malloc(sizeof(ei_rect_t));

              /* Taking into account the clipper : dest_rect and & text_rect
               * must be of the same size.
               * Example: if the width of the text is larger than the clipper,
               * the width of the clipper is taken. */
              ei_point_t point = *where;
              if (clipper != NULL){
                      const ei_rect_t clipper_window = hw_surface_get_rect(surface);
                      ei_size_t size_rect = {width, height};
                      dest_rect->size = size_rect;
                      dest_rect->top_left = point;
                      update_clipper(dest_rect, &clipper_window);
                      update_clipper(dest_rect, clipper);
              }

              /* Calculation of the source rectangle */
              ei_surface_t *text_surface = malloc(sizeof(ei_surface_t));
              *text_surface = hw_text_create_surface(text, font, color);
              ei_rect_t text_rect = hw_surface_get_rect(*text_surface);

              int incr_x = dest_rect->top_left.x - point.x;
              int incr_y = dest_rect->top_left.y - point.y;;
              text_rect.top_left.x = text_rect.top_left.x + incr_x;
              text_rect.top_left.y = text_rect.top_left.y + incr_y;
              text_rect.size.width = dest_rect->size.width;
              text_rect.size.height = dest_rect->size.height;
              /* Copy of the pixels */
              ei_copy_surface(surface, dest_rect, *text_surface, &text_rect, EI_TRUE);

              free(dest_rect);
              free(text_surface);

              hw_surface_unlock(surface);
              hw_surface_update_rects(surface, NULL);
}

ei_color_t*      ei_transparency(ei_color_t destination_color, ei_color_t source_color)
{
        /* Calculates the new value of a pixel when transparency is effective
         */

        ei_color_t* src_color = malloc(sizeof(ei_color_t));
        ei_color_t* dst_color = malloc(sizeof(ei_color_t));
        ei_color_t* resulting_color = malloc(sizeof(ei_color_t));
        *src_color = source_color;
        *dst_color = destination_color;

        resulting_color->red = (src_color->red * src_color->alpha + (255 - src_color->alpha) * dst_color->red) / 255;
        resulting_color->blue = (src_color->blue * src_color->alpha + (255 - src_color->alpha) * dst_color->blue) / 255;
        resulting_color->green = (src_color->green * src_color->alpha + (255 - src_color->alpha) * dst_color->green) / 255;
        resulting_color->alpha = dst_color->alpha;

        free(src_color);
        free(dst_color);

        return(resulting_color);
}

int			  ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
         /* Copies a surface, or a subpart, to another one.
          */

         hw_surface_lock(source);
         hw_surface_lock(destination);

         ei_size_t size_source = hw_surface_get_size(source);
         ei_size_t size_destination = hw_surface_get_size(destination);
         ei_size_t size_rect_source = size_source;
         ei_size_t size_rect_dest = size_destination;
         uint32_t xmin_src = 0;
         uint32_t ymin_src = 0;

         if (src_rect != NULL){
                 xmin_src = src_rect->top_left.x;
                 ymin_src = src_rect->top_left.y;
                 size_rect_source = src_rect->size;
         }
         uint32_t xmin_dest = 0;
         uint32_t ymin_dest = 0;
         if (dst_rect != NULL){
                 xmin_dest = dst_rect->top_left.x;
                 ymin_dest = dst_rect->top_left.y;
                 size_rect_dest = dst_rect->size;
         }
         if (size_rect_source.width != size_rect_dest.width || size_rect_source.height != size_rect_dest.height){
                 hw_surface_unlock(source);
                 hw_surface_unlock(destination);
                 return(1);
         }
         else {
               uint32_t *pixel_source = (uint32_t*)hw_surface_get_buffer(source);
               uint32_t *pixel_destination = (uint32_t*)hw_surface_get_buffer(destination);
               if (alpha){
                       for (int y = 0; y < size_rect_source.height; y++){
                               for (int x = 0; x < size_rect_source.width; x++){
                                       ei_color_t src_color = ei_demap_rgba(source, pixel_source[x +xmin_src + size_source.width*(y + ymin_src)]);
                                       ei_color_t dst_color = ei_demap_rgba(destination, pixel_destination[x + xmin_dest + size_destination.width*(y + ymin_dest)]);

                                       ei_color_t* new_color = ei_transparency(dst_color, src_color);
                                       pixel_destination[x + xmin_dest + size_destination.width*(y + ymin_dest)] = ei_map_rgba(destination, new_color);
                                       free(new_color);
                               }
                       }
               }
               else {
                       for (int y = ymin_src; y < size_rect_source.height; y++){
                               for (int x = xmin_src; x < size_rect_source.width; x++){
                                      pixel_destination[x + xmin_dest + size_destination.width*(y + ymin_dest)] = pixel_source[x +xmin_src + size_source.width*(y + ymin_src)];
                               }
                       }
               }

               hw_surface_unlock(source);
               hw_surface_unlock(destination);
               hw_surface_update_rects(source, NULL);
               hw_surface_update_rects(destination, NULL);

               return(0);
        }
}


/* --- Printing functions : used for debug only --- */

void printf_tab(struct polygon_side **tab, int length_tab)
{
        printf("longueur du tableau : %d\n", length_tab);
        for (int i = 0; i < length_tab; i++){
                printf(" impression case %d : \n", i);
                struct polygon_side *ptr = tab[i];
                while (ptr != NULL){
                      printf("%p\n", ptr);
                      ptr = ptr->next_side;
                }
        }
        printf("fin\n");
}

void printf_point(ei_point_t point)
{
        printf("point = (%d, %d) \n", point.x, point.y);
}
