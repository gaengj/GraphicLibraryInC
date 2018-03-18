#include "ei_types.h"
#include "ei_widgetclass.h"
#include "ei_widgetclass_intern.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include "ei_button_intern.h"
#include "ei_toplevel_intern.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definition of the array which makes the link between widget and class */
ei_widgetclass_t *TAB_WIDGETCLASS[10];

void update_clipper(ei_rect_t *first_clipper, const ei_rect_t *second_clipper)
{
        /* Transform the first clipper into a new clipper wich is
         * the intersection of the first and the second clipper
         */
        int abcisse_min_f, abcisse_max_f, abcisse_min_s, abcisse_max_s;
        abcisse_min_f = first_clipper->top_left.x;
        abcisse_max_f = abcisse_min_f + first_clipper->size.width;
        abcisse_min_s = second_clipper->top_left.x;
        abcisse_max_s = abcisse_min_s + second_clipper->size.width;

        if (abcisse_min_f < abcisse_min_s)
              abcisse_min_f = abcisse_min_s;
        if (abcisse_max_f > abcisse_max_s)
              abcisse_max_f = abcisse_max_s;

        int ordonnee_min_f, ordonnee_max_f, ordonnee_min_s, ordonnee_max_s;
        ordonnee_min_f = first_clipper->top_left.y;
        ordonnee_max_f = ordonnee_min_f + first_clipper->size.height;
        ordonnee_min_s = second_clipper->top_left.y;
        ordonnee_max_s = ordonnee_min_s + second_clipper->size.height;

        if (ordonnee_min_f < ordonnee_min_s)
              ordonnee_min_f = ordonnee_min_s;
        if (ordonnee_max_f > ordonnee_max_s)
              ordonnee_max_f = ordonnee_max_s;

        first_clipper->top_left.x = abcisse_min_f;
        first_clipper->top_left.y = ordonnee_min_f;
        first_clipper->size.width = abcisse_max_f - abcisse_min_f;
        first_clipper->size.height =  ordonnee_max_f - ordonnee_min_f;
}

void ei_widgetclass_register(ei_widgetclass_t* widgetclass)
{
        /* Registers a class to the program so that widgets of this class can
         * be created.
         */

        int i = 0;
        if (TAB_WIDGETCLASS[i] == NULL){
                TAB_WIDGETCLASS[i] = widgetclass;
        } else{
                while(TAB_WIDGETCLASS[i+1] != NULL){i++;};
                TAB_WIDGETCLASS[i]->next = widgetclass;
                TAB_WIDGETCLASS[i+1] = widgetclass;
        }
}

ei_widgetclass_t*	ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
        /* Returns the structure describing a class, from its name.
        */

        /* We search the array to find the class corresponding to the name
         * entered.*/
        int i = 0;
        while (TAB_WIDGETCLASS[i] != NULL && strcmp((TAB_WIDGETCLASS[i])->name, name))
                i++;
        if (i < 10 && TAB_WIDGETCLASS[i] != NULL){
                return TAB_WIDGETCLASS[i];
        } else {
                return NULL;
        }
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- REGISTER CLASS ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void	ei_frame_register_class()
{
        /* Registers the "frame" widget class in the program.
         */

        ei_widgetclass_t *class_frame = malloc(sizeof(ei_widgetclass_t));
        strcpy(class_frame->name, "frame");
        class_frame->allocfunc = &frame_allocfunc;
        class_frame->releasefunc = &frame_releasefunc;
        class_frame->drawfunc = &frame_drawfunc;
        class_frame->setdefaultsfunc = &frame_setdefaultsfunc;
        class_frame->geomnotifyfunc = &frame_geomnotifyfunc;
        class_frame->handlefunc = &frame_handlefunc;
        class_frame->next = NULL;
        ei_widgetclass_register(class_frame);
}

void	ei_button_register_class()
{
        /* Registers the "button" widget class in the program.
         */

        ei_widgetclass_t *class_button = malloc(sizeof(ei_widgetclass_t));
        strcpy(class_button->name, "button");
        class_button->allocfunc = &button_allocfunc;
        class_button->releasefunc = &button_releasefunc;
        class_button->drawfunc = &button_drawfunc;
        class_button->setdefaultsfunc = &button_setdefaultsfunc;
        class_button->geomnotifyfunc = &button_geomnotifyfunc;
        class_button->handlefunc = &button_handlefunc;
        class_button->next = NULL;
        ei_widgetclass_register(class_button);
}

void	ei_toplevel_register_class()
{
        /* Registers the "toplevel" widget class in the program.
         */

        ei_widgetclass_t *class_toplevel = malloc(sizeof(ei_widgetclass_t));
        strcpy(class_toplevel->name, "toplevel");
        class_toplevel->allocfunc = &toplevel_allocfunc;
        class_toplevel->releasefunc = &toplevel_releasefunc;
        class_toplevel->drawfunc = &toplevel_drawfunc;
        class_toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
        class_toplevel->geomnotifyfunc = &toplevel_geomnotifyfunc;
        class_toplevel->handlefunc = &toplevel_handlefunc;
        class_toplevel->next = NULL;
        ei_widgetclass_register(class_toplevel);
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- ALLOCFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void *frame_allocfunc()
{
        /* A function that allocates a block of memory that is big enough to
         * store the attributes of the frame widget.
         */

        return calloc(1, sizeof(frame_widget));
}

void *button_allocfunc()
{
        /* A function that allocates a block of memory that is big enough to
         * store the attributes of the frame widget.
         */

        return calloc(1, sizeof(button_widget));
}

void *toplevel_allocfunc()
{
        /* A function that allocates a block of memory that is big enough to
         * store the attributes of the frame widget.
         */

        return calloc(1, sizeof(toplevel_widget));
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- RELEASEFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void frame_releasefunc(struct ei_widget_t*	widget)
{
        /* A function that releases the memory used by the frame widget before
         * it is destroyed.
         */

        /* Nothing to be done in this case */
}

void button_releasefunc(struct ei_widget_t*	widget)
{
        /* A function that releases the memory used by the button widget before
         * it is destroyed.
         */

        /* TODO */
}

void toplevel_releasefunc(struct ei_widget_t*	widget)
{
        /* A function that releases the memory used by the toplevel widget before
         * it is destroyed.
         */

        /* TODO */
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- DRAWFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void frame_drawfunc(struct ei_widget_t*	widget, ei_surface_t	surface,
							      ei_surface_t		pick_surface, ei_rect_t*	clipper)
{
        /* A function that draws the frame widget
         */

        /* TODO : pick surface */

        /* We cast the widget into a frame widget */
				frame_widget *frame = (frame_widget *)widget;
        const ei_point_t point = {0,0};

        /* The drawing of a frame uses the draw_button function : a frame is a
         * button with a radius of rounded corners equal to 0 */
        draw_button(surface, &(widget->screen_location), 0.0, frame->border_width, EI_TRUE, EI_TRUE,
										&point, frame->text, frame->text_font, frame->text_color, frame->color, clipper);
        // draw_button(pick_surface, &(widget->screen_location), 0.0, frame->border_width, EI_TRUE, EI_TRUE,
				// 						&point, NULL, frame->text_font, frame->text_color, frame->color, clipper);
}

void button_drawfunc(struct ei_widget_t*	widget, ei_surface_t	surface,
							      ei_surface_t		pick_surface, ei_rect_t*	clipper)
{
        /* A function that draws the button widget
         */

        /* TODO : pick surface */

        /* We cast the widget into a button widget */
				button_widget *button = (button_widget *)widget;
        const ei_point_t point = {0,0};

        double radius = (double) button->corner_radius;
        draw_button(surface, &(widget->screen_location), radius, button->border_width, EI_TRUE, EI_TRUE,
										&point, button->text, button->text_font, button->text_color, button->color, clipper);
        // draw_button(pick_surface, &(widget->screen_location), 0.0, frame->border_width, EI_TRUE, EI_TRUE,
				// 						&point, NULL, frame->text_font, frame->text_color, frame->color, clipper);
}

void toplevel_drawfunc(struct ei_widget_t*	widget, ei_surface_t	surface,
							      ei_surface_t		pick_surface, ei_rect_t*	clipper)
{
        /* A function that draws the toplevel widget
         */

        /* TODO : pick surface */

        /* We cast the widget into a toplevel widget */
				toplevel_widget *toplevel = (toplevel_widget *)widget;

        const ei_point_t point = {0,0};
        /* A definir ici car on l'utilise pas dans toplevel configure*/
        ei_font_t	font = ei_default_font;
        ei_color_t color_font = {255, 255, 255, 255};

        draw_toplevel(surface, &(widget->screen_location), &point, toplevel->title, font, color_font,
                      toplevel->color, toplevel->border_width, clipper);
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- SETDEFAULTSFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void	frame_setdefaultsfunc(struct ei_widget_t*	widget)
{
        /* A function that sets the default values for the class frame
         */

        ei_size_t size = widget->requested_size;
        if (size.width == 0 || size.height == 0){
                ei_size_t new_size = {100, 100};
                widget->requested_size = new_size;
        }


        ei_size_t rect_size = widget->screen_location.size;
        if (rect_size.width == 0 || rect_size.height == 0){
                ei_rect_t new_rectangle;
                ei_point_t point = {0, 0};
                new_rectangle.top_left = point;
                ei_size_t new_rect_size = {100, 100};
                new_rectangle.size = new_rect_size;
                widget->screen_location = new_rectangle;
        }

        if (widget->content_rect == NULL){
                widget->content_rect = &(widget->screen_location);
        }
        frame_widget *frame = (frame_widget *)widget;

        ei_color_t color_default = {100, 100, 100, 255} ;
        frame->color = color_default;

        int border_default = 0;
        frame->border_width = border_default;

        ei_relief_t relief = ei_relief_none;
        frame->relief = relief;

        ei_font_t font = ei_default_font;
        frame->text_font = font;

        ei_color_t text_color_default = {255, 255, 255, 255};
        frame->text_color = text_color_default;

        ei_anchor_t anchor_text = ei_anc_north;
        frame->text_anchor = anchor_text;

        // frame->img =
        //         ei_rect_t rect_img_default = hw_surface_get_rect(*(frame->img));
        //         ei_rect_t *rect_img_default_ad = &rect_img_default;
        //         ei_rect_t **rect_img_default_adress = &rect_img_default_ad;
        //         frame->img_rect = rect_img_default_adress;

        ei_anchor_t anchor_img = ei_anc_center;
        frame->img_anchor = anchor_img;
}

void	button_setdefaultsfunc(struct ei_widget_t*	widget)
{
        /* A function that sets the default values for the class button
         */
         ei_size_t size = widget->requested_size;
         if (size.width == 0 || size.height == 0){
                 ei_size_t new_size = {100, 100};
                 widget->requested_size = new_size;
         }


         ei_size_t rect_size = widget->screen_location.size;
         if (rect_size.width == 0 || rect_size.height == 0){
                 ei_rect_t new_rectangle;
                 ei_point_t point = {0, 0};
                 new_rectangle.top_left = point;
                 ei_size_t new_rect_size = {100, 100};
                 new_rectangle.size = new_rect_size;
                 widget->screen_location = new_rectangle;
         }
         if (widget->content_rect == NULL){
                 widget->content_rect = &(widget->screen_location);
         }

         button_widget *button = (button_widget *)widget;

         ei_color_t color_default = {120, 100, 150, 255} ;
         button->color = color_default;

         int border_default = 5;
         button->border_width = border_default;

         int corner_radius_default = 10;
         button->corner_radius = corner_radius_default;

         ei_relief_t relief = ei_relief_none;
         button->relief = relief;

         ei_font_t font = ei_default_font;
         button->text_font = font;

         ei_color_t text_color_default = {0, 0, 0, 255};
         button->text_color = text_color_default;

         ei_anchor_t anchor_text = ei_anc_north;
         button->text_anchor = anchor_text;

         // button->img =
         //         ei_rect_t rect_img_default = hw_surface_get_rect(*(button->img));
         //         ei_rect_t *rect_img_default_ad = &rect_img_default;
         //         ei_rect_t **rect_img_default_adress = &rect_img_default_ad;
         //         button->img_rect = rect_img_default_adress;

         ei_anchor_t anchor_img = ei_anc_center;
         button->img_anchor = anchor_img;

        //  button->callback =
        //
        //  button->user_param =
}

void	toplevel_setdefaultsfunc(struct ei_widget_t*	widget)
{
        /* A function that sets the default values for the class toplevel
         */
         ei_size_t size = widget->requested_size;
         if (size.width == 0 || size.height == 0){
                 ei_size_t new_size = {100, 100};
                 widget->requested_size = new_size;
         }

         ei_size_t rect_size = widget->screen_location.size;
         if (rect_size.width == 0 || rect_size.height == 0){
                 ei_rect_t new_rectangle;
                 ei_point_t point = {0, 0};
                 new_rectangle.top_left = point;
                 ei_size_t new_rect_size = {100, 100};
                 new_rectangle.size = new_rect_size;
                 widget->screen_location = new_rectangle;
         }
         if (widget->content_rect == NULL){
                 widget->content_rect = &(widget->screen_location);
         }

         toplevel_widget *toplevel = (toplevel_widget *)widget;

         ei_color_t color_default = {120, 120, 120, 255} ;
         toplevel->color = color_default;

         int border_default = 3;
         toplevel->border_width = border_default;

         char * title_default = "New toplevel";
         toplevel->title = title_default;

         ei_bool_t close_default = EI_FALSE;
         toplevel->closable = close_default;

         ei_axis_set_t axis_default = ei_axis_none;
         toplevel->resizable = axis_default;

         ei_size_t size_default = {300, 300};
         toplevel->min_size = &size_default;
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- GEOMNOTIFYFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void frame_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect)
{
        /* A function that is called to notify the frame widget that its
         * geometry has been modified by its geometry manager.
         */

        /* TODO */
}

void button_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect)
{
        /* A function that is called to notify the button widget that its
         * geometry has been modified by its geometry manager.
         */

        /* TODO */
}

void toplevel_geomnotifyfunc(struct ei_widget_t*	widget, ei_rect_t rect)
{
        /* A function that is called to notify the toplevel widget that its
         * geometry has been modified by its geometry manager.
         */

        /* TODO */
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- HANDLEFUNC ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

ei_bool_t frame_handlefunc(struct ei_widget_t*	widget,	 struct ei_event_t*	event)
{
        /* A function that is called in response to an event affecting the
         * frame.
         */

        /* TODO */
        return EI_FALSE;
}

ei_bool_t button_handlefunc(struct ei_widget_t*	widget,	 struct ei_event_t*	event)
{
        /* A function that is called in response to an event affecting the
         * button.
         */

        /* TODO */
        return EI_FALSE;
}

ei_bool_t toplevel_handlefunc(struct ei_widget_t*	widget,	 struct ei_event_t*	event)
{
        /* A function that is called in response to an event affecting the
         * toplevel.
         */

        /* TODO */
        return EI_FALSE;
}
