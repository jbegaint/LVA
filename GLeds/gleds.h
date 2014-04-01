#ifndef _GLEDS_H_
#define _GLEDS_H_

#define GLADE_FILE "GLeds/Gleds.glade"

#define LED0 "GLeds/led_full.png"
#define LED1 "GLeds/led_1.png"
#define LED2 "GLeds/led_2.png"
#define LED3 "GLeds/led_black.png"

typedef struct _DataWrapper
{
	GtkWidget *grid;
	GtkWidget *start_btn;
	GtkWidget *pause_btn;
} DataWrapper;

#endif