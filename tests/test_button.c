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

	ei_widget_t*	button;
	ei_size_t	button_size		= {300,200};
	int		button_x			= 0;
	int		button_y			= 0;
	ei_color_t	button_color		= {150, 100, 70, 0xff};
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 6;
	int radius = 10;
	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the button on screen. */
	button = ei_widget_create("button", ei_app_root_widget());

	ei_color_t	text_color		= {0xff, 0xff, 0xff, 0xff};
	char	*text	= "First button";
	ei_font_t text_font = ei_default_font;
	ei_button_configure	(button, &button_size, &button_color, &button_border_width,
		&radius, &button_relief, &text, &text_font, &text_color, NULL,
				 NULL, NULL, NULL, NULL, NULL);

	ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
