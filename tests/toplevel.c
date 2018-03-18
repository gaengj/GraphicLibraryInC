#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_placer.h"
#include "ei_widgetclass_intern.h"
/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	toplevel;
	ei_size_t	toplevel_size		= {300,200};
	int		toplevel_x			= 100;
	int		toplevel_y			= 100;
	ei_color_t	toplevel_color		= {0x88, 0x88, 0x88, 0xff};
	int		toplevel_border_width	= 6;
	ei_bool_t close = EI_FALSE;
	ei_axis_set_t axis = ei_axis_none;
	ei_size_t min_size = {100, 100};
	ei_size_t *addr_min_size = &min_size;
	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the toplevel on screen. */
	toplevel = ei_widget_create("toplevel", ei_app_root_widget());

	char	*title	= "First toplevel";
	ei_toplevel_configure	(toplevel, &toplevel_size, &toplevel_color,
				 &toplevel_border_width, &title, &close, &axis, &addr_min_size);

	ei_place(toplevel, NULL, &toplevel_x, &toplevel_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
