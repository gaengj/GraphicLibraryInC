#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "ei_placer.h"
#include "ei_placer_intern.h"
#include "ei_types.h"
#include "ei_widget.h"


void ei_place(struct ei_widget_t*	widget,
	 						ei_anchor_t* anchor,
							int* x,
							int* y,
							int* width,
							int* height,
							float* rel_x,
							float* rel_y,
							float* rel_width,
							float* rel_height)
{
				/* Configures the geometry of a widget using the "placer" geometry manager.
				 */

				struct ei_widget_t* parent = widget->parent;
				update_placer_params(widget, anchor, x, y, width, height, rel_x, rel_y, rel_width, rel_height);
				find_width(widget, parent, width, rel_width);
				find_height(widget, parent, height, rel_height);
				find_topleft_x(widget, parent, anchor, x, rel_x);
				find_topleft_y(widget, parent, anchor, y, rel_y);
}


void ei_placer_run(struct ei_widget_t* widget)
{
				/* Tells the placer to recompute the geometry of a widget.
				 */
				struct ei_widget_t* parent = widget->parent;
				ei_placer_params_t* placer_params = widget->placer_params;
				if (placer_params != NULL) {
								ei_anchor_t* anchor = placer_params->anchor;
								int* x = placer_params->x;
								int* y = placer_params->y;
								int* w = placer_params->w;
								int* h = placer_params->h;
								float* rx = placer_params->rx;
								float* ry = placer_params->ry;
								float* rw = placer_params->rw;
								float* rh = placer_params->rh;

								find_width(widget, parent, w, rw);
								find_height(widget, parent, h, rh);
								find_topleft_x(widget, parent, anchor, x, rx);
								find_topleft_y(widget, parent, anchor, y, ry);
			  }
}


void ei_placer_forget(struct ei_widget_t* widget)
{
				/* Tells the placer to remove a widget from the screen and forget about it.
				 */
        widget->placer_params = NULL;
}


void update_placer_params(struct ei_widget_t*	widget,
												  ei_anchor_t* anchor,
												  int* x,
												  int* y,
												  int* width,
												  int* height,
												  float* rel_x,
												  float* rel_y,
												  float* rel_width,
												  float* rel_height)
{
				/* Update the placer parameters whose arguments are not NULL
				 */
				ei_placer_params_t* params;
				if (widget->placer_params == NULL){
								params = malloc(sizeof(ei_placer_params_t));
			  }
				else {
								params = widget->placer_params;
			  }
				if (anchor != NULL){
								params->anchor = anchor;
								params->anchor_data = *anchor;
				}

				if (x != NULL){
								params->x = x;
								params->x_data = *x;
				}

				if (y != NULL){
								params->y = y;
								params->y_data = *y;
				}

				if (width != NULL){
								params->w = width;
								params->w_data = *width;
				}

				if (height != NULL){
								params->h = height;
								params->h_data = *height;
				}

				if (rel_x != NULL){
								params->rx = rel_x;
								params->rx_data = *rel_x;
				}

				if (rel_y != NULL){
								params->ry = rel_y;
								params->ry_data = *rel_y;
				}

				if (rel_width != NULL){
								params->rw = rel_width;
								params->rw_data = *rel_width;
				}

				if (rel_height != NULL){
								params->rh = rel_height;
								params->rh_data = *rel_height;
				}
				widget->placer_params = params;
}

void find_topleft_x(struct ei_widget_t* widget, struct ei_widget_t* parent, ei_anchor_t* anchor, int* x, float* rel_x)
{
				/* Calculate the top-left corner abscissa of the widget
				 */
				int *x_new = malloc(sizeof(int));
				if (x == NULL)
								*x_new = 0;
				else
								*x_new = *x;

				float *rel_x_new = malloc(sizeof(float));
				if (rel_x == NULL)
								*rel_x_new = 0.0;
				else
								*rel_x_new = *rel_x;

				if (anchor == NULL) {
								/* default is northwest */
								widget->screen_location.top_left.x = *x_new + ((*rel_x_new)*parent->screen_location.size.width);
				}
				else {
								switch (*anchor) {
												case ei_anc_center:
												case ei_anc_north:
												case ei_anc_south:
																widget->screen_location.top_left.x = *x_new + ((*rel_x_new)*parent->screen_location.size.width) - (widget->screen_location.size.width)/2;
																break;
												case ei_anc_northeast:
												case ei_anc_east:
												case ei_anc_southeast:
																widget->screen_location.top_left.x = *x_new + ((*rel_x_new)*parent->screen_location.size.width) - (widget->screen_location.size.width);
																break;
												default:
																widget->screen_location.top_left.x = *x_new + ((*rel_x_new)*parent->screen_location.size.width);
																break;
								}
			  }
				free(x_new);
				free(rel_x_new);
}

void find_topleft_y(struct ei_widget_t* widget, struct ei_widget_t* parent, ei_anchor_t* anchor, int* y, float* rel_y)
{
				/* Calculate the top-left corner ordinate of the widget
				 */
				int *y_new = malloc(sizeof(int));
				if (y == NULL)
								*y_new = 0;
				else
								*y_new = *y;

				float *rel_y_new = malloc(sizeof(float));
				if (rel_y == NULL)
								*rel_y_new = 0.0;
				else
								*rel_y_new = *rel_y;

				if (anchor == NULL) {
								/* default is northwest */
								widget->screen_location.top_left.y = *y_new + ((*rel_y_new)*parent->screen_location.size.height);
				}
				else {
								switch (*anchor) {
												case ei_anc_center:
												case ei_anc_west:
												case ei_anc_east:
																widget->screen_location.top_left.y = *y_new + ((*rel_y_new)*parent->screen_location.size.height) - (widget->screen_location.size.height)/2;
																break;
												case ei_anc_southeast:
												case ei_anc_south:
												case ei_anc_southwest:
																widget->screen_location.top_left.y = *y_new + ((*rel_y_new)*parent->screen_location.size.height) - (widget->screen_location.size.height);
																break;
												default:
																widget->screen_location.top_left.y = *y_new + ((*rel_y_new)*parent->screen_location.size.height);
																break;
								}
			  }
				free(y_new);
				free(rel_y_new);
}

void find_width(struct ei_widget_t* widget, struct ei_widget_t* parent, int* width, float* rel_width)
{
				/* Calculate the width of the widget
				 */
				if (width != NULL){
								widget->screen_location.size.width = *width;
				}
				else if (rel_width != NULL) {
								widget->screen_location.size.width = (int)(parent->screen_location.size.width*(*rel_width));
				}
				else {
								widget->screen_location.size.width = widget->requested_size.width;
				}
}

void find_height(struct ei_widget_t* widget, struct ei_widget_t* parent, int* height, float* rel_height)
{
				/* Calculate the height of the widget
				 */
				if (height != NULL){
								widget->screen_location.size.height = *height;
				}
				else if (rel_height != NULL) {
								widget->screen_location.size.height = (int)(parent->screen_location.size.height*(*rel_height));
				}
				else {
								widget->screen_location.size.height = widget->requested_size.height;
				}
}
