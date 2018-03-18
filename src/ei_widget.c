#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_widgetclass_intern.h"
#include "ei_placer.h"
#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ei_application_intern.h"

ei_widget_t *ei_widget_create(ei_widgetclass_name_t	class_name, ei_widget_t *parent)
{
			/* Creates a new instance of a widget of some particular class, as a
			 * descendant of an existing widget.
			 */

			if (parent == NULL){
							return NULL;
			} else {
							ei_widgetclass_t *class = ei_widgetclass_from_name(class_name);
							if (class != NULL){

											ei_widgetclass_allocfunc_t allocfunc = class->allocfunc;
											ei_widget_t *new_widget = (*allocfunc)();
											new_widget->wclass = class;
											ei_widgetclass_setdefaultsfunc_t setdefaultsfunc = class->setdefaultsfunc;
											(*setdefaultsfunc)(new_widget);

											/* We make the link with the widget parent*/
											if (parent->children_tail != NULL){
															parent->children_tail->next_sibling = new_widget;
											} else {
															parent->children_head = new_widget;
											}
											parent->children_tail = new_widget;
											new_widget->parent = parent;
											return new_widget;
							}
							else{
											return NULL;
							}
			}
}

void ei_widget_destroy(ei_widget_t *widget)
{
					/* Destroys a widget. Removes it from screen if it is managed by a
					 * geometry manager. Destroys all its descendants.
					 */

					ei_widgetclass_t *class = widget->wclass;
				 	ei_widgetclass_releasefunc_t releasefunc = class->releasefunc;
					(*releasefunc)(widget);
					free(widget);
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- WIDGET_CONFIGURE ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void ei_frame_configure	(ei_widget_t *widget,
							 ei_size_t *requested_size,
							 const ei_color_t *color,
							 int *border_width,
							 ei_relief_t *relief,
							 char **text,
							 ei_font_t *text_font,
							 ei_color_t *		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor)
{
				/* Configures the attributes of widgets of the class "frame".
				 */

				if (widget != NULL){
								ei_widgetclass_t *class = widget->wclass;
								ei_widgetclass_setdefaultsfunc_t setdefaultsfunc = class->setdefaultsfunc;
								(*setdefaultsfunc)(widget);

								if (requested_size != NULL)
													widget->requested_size = *requested_size;

								frame_widget *frame = (frame_widget *)widget;
								if (color != NULL)
												frame->color = *color;

								if (border_width != NULL)
												frame->border_width = *border_width;
								if (relief != NULL)
												frame->relief = *relief;
								if (text != NULL)
												frame->text = *text;
								if (text_font != NULL)
												frame->text_font = *text_font;
								if (text_color != NULL)
												frame->text_color = *text_color;
								if (text_anchor != NULL)
												frame->text_anchor = *text_anchor;
								if (img != NULL)
												frame->img = *img;
								if (img_rect != NULL)
												frame->img_rect = *img_rect;
								if (img_anchor != NULL)
												frame->img_anchor = *img_anchor;
				}
}

void			ei_button_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,

							 int*			corner_radius,

							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor,

							 ei_callback_t*		callback,
							 void**			user_param)
{
				/* Configures the attributes of widgets of the class "button".
				 */

				if (widget != NULL){
								ei_widgetclass_t *class = widget->wclass;
								ei_widgetclass_setdefaultsfunc_t setdefaultsfunc = class->setdefaultsfunc;
								(*setdefaultsfunc)(widget);

								if (requested_size != NULL)
													widget->requested_size = *requested_size;

								button_widget *button = (button_widget *)widget;

								if (color != NULL)
												button->color = *color;
								if (border_width != NULL)
												button->border_width = *border_width;

								if (corner_radius != NULL)
												button->corner_radius = *corner_radius;

								if (relief != NULL)
												button->relief = *relief;
								if (text != NULL)
												button->text = *text;
								if (text_font != NULL)
												button->text_font = *text_font;
								if (text_color != NULL)
												button->text_color = *text_color;
								if (text_anchor != NULL)
												button->text_anchor = *text_anchor;
								if (img != NULL)
												button->img = *img;
								if (img_rect != NULL)
												button->img_rect = *img_rect;
								if (img_anchor != NULL)
												button->img_anchor = *img_anchor;
								if (callback != NULL)
												button->callback = *callback;
								if (user_param != NULL)
												button->user_param = *user_param;
				}
}

void			ei_toplevel_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,

							 char**			title,
							 ei_bool_t*		closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_t**		min_size)
{
	/* Configures the attributes of widgets of the class "toplevel".
	 */

				if (widget != NULL){
								ei_widgetclass_t *class = widget->wclass;
								ei_widgetclass_setdefaultsfunc_t setdefaultsfunc = class->setdefaultsfunc;
								(*setdefaultsfunc)(widget);

								if (requested_size != NULL)
													widget->requested_size = *requested_size;

								toplevel_widget *toplevel = (toplevel_widget *)widget;
								if (color != NULL)
												toplevel->color = *color;

								if (border_width != NULL)
												toplevel->border_width = *border_width;

								if (title != NULL)
												toplevel->title = *title;
								if (closable != NULL)
												toplevel->closable = *closable;
								if (resizable != NULL)
												toplevel->resizable = *resizable;
								if (min_size != NULL)
												toplevel->min_size = *min_size;
				}
}
