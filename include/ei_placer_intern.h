#include <stdint.h>
#include "ei_placer.h"
#include "ei_types.h"
#include "ei_widget.h"

/* Update the placer parameters
 */
void update_placer_params(struct ei_widget_t*	widget,
							 ei_anchor_t*		anchor,
							 int*			x,
							 int*			y,
							 int*			width,
							 int*			height,
							 float*			rel_x,
							 float*			rel_y,
							 float*			rel_width,
							 float*			rel_height);

/* Calculate the width of the widget
 */
void find_width(struct ei_widget_t* widget, struct ei_widget_t* parent, int* width, float* rel_width);

/* Calculate the height of the widget
 */
void find_height(struct ei_widget_t* widget, struct ei_widget_t* parent, int* height, float* rel_height);

/* Calculate the top-left corner abscissa of the widget
 */
void find_topleft_x(struct ei_widget_t* widget, struct ei_widget_t* parent, ei_anchor_t* anchor, int* x, float* rel_x);

/* Calculate the top-left corner ordinate of the widget
 */
void find_topleft_y(struct ei_widget_t* widget, struct ei_widget_t* parent, ei_anchor_t* anchor, int* y, float* rel_y);
