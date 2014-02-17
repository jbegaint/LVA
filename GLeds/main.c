#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h> 

#define GLADE_FILE "Gleds.glade"

#define LED0 "led_full.png"
#define LED1 "led_1.png"
#define LED2 "led_black.png"

#define N_ROWS 14
#define N_COLS 25

char *LED_ARRAY[3] = {LED0, LED1, LED2}; 

typedef struct _data {
	GtkWidget *start_button;
	GtkWidget *pause_button;
	GtkWidget *grid;
};

void quit(void)
{
	g_print("Bye!\n");
	gtk_main_quit();
}

void on_open_activate(GtkWidget *widget, gpointer user_data)
{
	g_print("%s clicked!\n", gtk_widget_get_name(widget));
}

void on_pause_clicked(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *launch_button = NULL;
	g_print("%s clicked!\n", gtk_widget_get_name(widget));
}

void on_launch_clicked(GtkWidget *widget, gpointer user_data)
{
	/*gtk_widget_set_sensitive(widget, FALSE);*/

	GList *children, *l;
	GtkWidget *grid = NULL;

	grid = GTK_WIDGET(user_data);
	gchar *led_state = NULL;

	children = gtk_container_get_children(GTK_CONTAINER(grid));

	for (l = children; l != NULL; l = l->next) {
		GtkWidget *image = l->data;
		g_object_get(image, "file", &led_state, NULL);

		if (!led_state) {
			gtk_image_set_from_file(GTK_IMAGE(image), LED0);
			continue;
		}
		else {
			gtk_image_set_from_file(GTK_IMAGE(image), LED_ARRAY[rand()%3]);
		}
	}

	g_list_free(children);
	g_free(led_state);
}

void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
	quit();
}

int main(int argc, char **argv) 
{ 
	GError *error = NULL; 
	GtkBuilder *builder = NULL; 
	gchar *filename = NULL;

	GtkWidget *main_window = NULL; 
	GtkWidget *led_matrix = NULL;
	GtkWidget *start_button = NULL;
	GtkWidget *pause_button = NULL;
	

	/* Gtk init */
	gtk_init(&argc, &argv); 

	/* open glade file */ 
	builder = gtk_builder_new(); 
	filename =  g_build_filename(GLADE_FILE, NULL); 

	/* load glade file */
	gtk_builder_add_from_file(builder, filename, &error); 
	g_free(filename); 

	if (error) {
		gint code = error->code; 
		g_printerr("%s\n", error->message); 
		g_error_free(error); 

		return code; 
	}

	/* get widgets */
	main_window = GTK_WIDGET(gtk_builder_get_object (builder, "main_window")); 
	led_matrix = GTK_WIDGET(gtk_builder_get_object (builder, "led_matrix"));

	start_button = GTK_WIDGET(gtk_builder_get_object (builder, "start_button")); 
	pause_button = GTK_WIDGET(gtk_builder_get_object (builder, "pause_button")); 

	/* connect signals */
    gtk_builder_connect_signals(builder, NULL);

    struct _data data;
    data.pause_button = pause_button;
    data.start_button = start_button;
    data.grid = led_matrix;

	g_signal_connect(start_button, "clicked", G_CALLBACK(on_launch_clicked), led_matrix);
	g_signal_connect(pause_button, "clicked", G_CALLBACK(on_pause_clicked), led_matrix);

	/* display main_window */
	gtk_widget_show_all(main_window); 

	gtk_main(); 

	return 0; 
} 