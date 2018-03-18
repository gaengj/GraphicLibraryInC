#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "hw_interface.h"
#include "ei_application.h"
#include "ei_application_intern.h"
#include <stdio.h>
#include <stdlib.h>

/* Definition of the array which makes the link between widget and class */
extern ei_widgetclass_t *TAB_WIDGETCLASS[10];

/* Transform the first clipper into a new clipper wich is
 * the intersection of the first and the second clipper
 */
void update_clipper(ei_rect_t *first_clipper, const ei_rect_t *second_clipper);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- DEFINITION OF STRUCTURES ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* Definition of the structure frame_widget */
typedef struct frame_widget {
			  ei_widget_t widget;
				ei_color_t	color;
				int			border_width;
				ei_relief_t		relief;
				char*			text;
				ei_font_t	text_font;
				ei_color_t	text_color;
				ei_anchor_t		text_anchor;
				ei_surface_t	img;
				ei_rect_t*	img_rect;
				ei_anchor_t	img_anchor;
} frame_widget;

typedef struct button_widget {
			  ei_widget_t widget;
				ei_color_t	color;
				int			border_width;

				int corner_radius;

				ei_relief_t		relief;
				char*			text;
				ei_font_t	text_font;
				ei_color_t	text_color;
				ei_anchor_t		text_anchor;
				ei_surface_t	img;
				ei_rect_t*	img_rect;
				ei_anchor_t	img_anchor;

				ei_callback_t		callback;
				void*			user_param;
} button_widget;

typedef struct toplevel_widget {
			  ei_widget_t widget;
				ei_color_t	color;
				int			border_width;

				char* title;
				ei_bool_t closable;
				ei_axis_set_t resizable;
				ei_size_t* min_size;
} toplevel_widget;

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- ALLOCFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* A function that allocates a block of memory that is big enough to
 * store the attributes of the frame widget.
 */
void *frame_allocfunc();

/* A function that allocates a block of memory that is big enough to
 * store the attributes of the frame widget.
 */
void *button_allocfunc();

/* A function that allocates a block of memory that is big enough to
 * store the attributes of the frame widget.
 */
void *toplevel_allocfunc();

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- RELEASEFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* A function that releases the memory used by the frame widget before
 * it is destroyed.
 */
void frame_releasefunc(struct ei_widget_t*	widget);

/* A function that releases the memory used by the button widget before
 * it is destroyed.
 */
void button_releasefunc(struct ei_widget_t*	widget);

/* A function that releases the memory used by the toplevel widget before
 * it is destroyed.
 */
void toplevel_releasefunc(struct ei_widget_t*	widget);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- DRAWFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* A function that draws the frame widget
 */
void frame_drawfunc(struct ei_widget_t*	widget, ei_surface_t	surface,
							      ei_surface_t		pick_surface, ei_rect_t*	clipper);

/* A function that draws the button widget
 */
void button_drawfunc(struct ei_widget_t*	widget, ei_surface_t	surface,
					      			ei_surface_t		pick_surface, ei_rect_t*	clipper);

/* A function that draws the toplevel widget
 */
void toplevel_drawfunc(struct ei_widget_t*	widget, ei_surface_t	surface,
							      ei_surface_t		pick_surface, ei_rect_t*	clipper);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- SETDEFAULTSFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* A function that sets the default values for the class frame
 */
void	frame_setdefaultsfunc(struct ei_widget_t*	widget);

/* A function that sets the default values for the class button
 */
void	button_setdefaultsfunc(struct ei_widget_t*	widget);

/* A function that sets the default values for the class toplevel
 */
void	toplevel_setdefaultsfunc(struct ei_widget_t*	widget);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- GEOMNOTIFYFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* A function that is called to notify the frame widget that its
 * geometry has been modified by its geometry manager.
 */
void frame_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect);

/* A function that is called to notify the button widget that its
 * geometry has been modified by its geometry manager.
 */
void button_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect);

/* A function that is called to notify the toplevel widget that its
 * geometry has been modified by its geometry manager.
 */
void toplevel_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect);

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- HANDLEFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

/* A function that is called in response to an event affecting the
 * frame.
 */
ei_bool_t frame_handlefunc(struct ei_widget_t*	widget,	 struct ei_event_t*	event);

/* A function that is called in response to an event affecting the
 * button.
 */
ei_bool_t button_handlefunc(struct ei_widget_t*	widget,	 struct ei_event_t*	event);

/* A function that is called in response to an event affecting the
 * toplevel.
 */
ei_bool_t toplevel_handlefunc(struct ei_widget_t*	widget,	 struct ei_event_t*	event);
