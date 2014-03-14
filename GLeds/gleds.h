#ifndef _GLEDS_H_
#define _GLEDS_H_

#define GLADE_FILE "Gleds.glade"

#define LED0 "led_full.png"
#define LED1 "led_1.png"
#define LED2 "led_2.png"
#define LED3 "led_black.png"

typedef struct _DataWrapper
{
	GtkWidget *grid;
	GtkWidget *start_btn;
	GtkWidget *pause_btn;
} DataWrapper;

#endif