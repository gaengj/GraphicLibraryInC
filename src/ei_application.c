#include "ei_types.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_widgetclass_intern.h"
#include "hw_interface.h"
#include "ei_application.h"
#include "ei_application_intern.h"
#include <stdio.h>
#include <stdlib.h>

ei_surface_t MAIN_WINDOW= NULL;
ei_surface_t OFFSCREEN= NULL;
frame_widget *ROOT_FRAME;
ei_widget_t *ROOT_WIDGET;

/*Event management*/
ei_bool_t QUIT_REQUEST = EI_FALSE;

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
        /* Creates an application.
        */

        hw_init();

        /* Creation of the root window */
        if (fullscreen == EI_TRUE){
                MAIN_WINDOW = hw_create_window(main_window_size, EI_TRUE);
        } else {
                MAIN_WINDOW = hw_create_window(main_window_size, EI_FALSE);
        }

        /* Registration of classes (and geometry manager ?) */
        ei_frame_register_class();
        ei_button_register_class();
        ei_toplevel_register_class();
        /* Creation of the root widget */
        ROOT_FRAME = calloc(1, sizeof(frame_widget));
        ROOT_WIDGET= (ei_widget_t*)ROOT_FRAME;
        ei_widgetclass_t *class = ei_widgetclass_from_name("frame");
        ROOT_WIDGET->wclass = class;
        ei_rect_t screen_location;
        ei_point_t point = {0, 0};
        screen_location.top_left = point;
        screen_location.size = *main_window_size;
        ROOT_WIDGET->screen_location = screen_location;
        hw_surface_update_rects(MAIN_WINDOW, NULL);
}

void ei_app_free()
{
        /* Releases all the resources of the application, and releases the
         * hardware.
         */

        /* Free all widget */
        ei_free_widget(ROOT_WIDGET);

        /* Free all classes */
        for (int i = 0; i < 10; i++){
                if (TAB_WIDGETCLASS[i] != NULL)
                        free(TAB_WIDGETCLASS[i]);
        }
        hw_quit();
}

void ei_free_widget(ei_widget_t * widget)
{
        /* Free the widget recursively
         */

        if (widget != NULL){
                /* Drawing of the children*/
                ei_widget_t *curr_children = widget->children_head;
                ei_widget_t *tail = widget->children_tail;
                while (curr_children != tail){
                        ei_free_widget(curr_children);
                }
                ei_free_widget(tail);
                ei_widget_destroy(widget);
       }
}

void ei_app_run()
{
        /*TODO : la stucture d'un programme evenementiel page 9-10 du poly
        tant que pas de demande d’arret du programme faire
        dessiner à l’ecran les mises à jour nécessaires
        attendre un evenement
        analyser l’evenement pour trouver le traitant associe
        appeler le traitant associe
        fin tant que*/

        /* Drawing of the root widget */
        ei_draw_widget(ei_app_root_widget());

        // while (QUIT_REQUEST == EI_FALSE){
        //
        // }
        getchar();
}

void ei_draw_widget(ei_widget_t * widget)
{
        /* Drawing of the widget*/
        if (widget != NULL){
                ei_rect_t rect;
                if (widget->parent != NULL)
                        rect = widget->parent->screen_location;
                else
                        rect = widget->screen_location;
                ei_widgetclass_drawfunc_t drawfunc = (widget->wclass)->drawfunc;
                (*drawfunc)(widget, MAIN_WINDOW, OFFSCREEN, &rect);
                /* Drawing of the children*/
                ei_widget_t *curr_children = widget->children_head;
                ei_widget_t *tail = widget->children_tail;
                while (curr_children != tail){
                        ei_draw_widget(curr_children);
                }
                ei_draw_widget(tail);
       }
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{
        /*TODO*/
}

void ei_app_quit_request()
{
        QUIT_REQUEST = EI_TRUE;
}

ei_widget_t* ei_app_root_widget()
{
        return ROOT_WIDGET;
}

ei_surface_t ei_app_root_surface()
{
        return MAIN_WINDOW;
}
