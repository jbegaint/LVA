#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#define GLADE_FILE "Gleds.glade"

#define LED0 "led_full.png"
#define LED1 "led_1.png"
#define LED2 "led_black.png"

#define N_ROWS 7
#define N_COLS 5

char *LED_ARRAY[3] = { LED0, LED1, LED2 };

static gboolean continue_loop;

typedef struct _DataWrapper
{
	GtkWidget *grid;
	GtkWidget *start_btn;
	GtkWidget *pause_btn;
} DataWrapper;

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

	DataWrapper *data = (DataWrapper *) user_data;

	gtk_widget_set_sensitive(data->start_btn, TRUE); 
	gtk_widget_set_sensitive(data->pause_btn, FALSE); 

	continue_loop = FALSE;
}

gboolean loop_random(gpointer user_data)
{
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
		} else {
			gtk_image_set_from_file(GTK_IMAGE(image), LED_ARRAY[rand() % 3]);
		}
	}

	g_list_free(children);
	g_free(led_state);

	return continue_loop;
}

void on_launch_clicked(GtkWidget *widget, gpointer user_data)
{
	continue_loop = TRUE;

	DataWrapper *data = (DataWrapper *) user_data;

	gtk_widget_set_sensitive(data->start_btn, FALSE); 
	gtk_widget_set_sensitive(data->pause_btn, TRUE); 

	g_timeout_add(33, loop_random, data->grid);
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
	filename = g_build_filename(GLADE_FILE, NULL);

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
	main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	led_matrix = GTK_WIDGET(gtk_builder_get_object(builder, "led_matrix"));

	start_button = GTK_WIDGET(gtk_builder_get_object(builder, "start_button"));
	pause_button = GTK_WIDGET(gtk_builder_get_object(builder, "pause_button"));

	DataWrapper *data = g_new(DataWrapper, 1);
	data->grid = led_matrix;
	data->start_btn = start_button;
	data->pause_btn = pause_button;

	for (int i = 0; i < N_COLS; i++) {
		for (int j = 0; j < N_ROWS; j++) {
			GtkWidget *image;
			image = gtk_image_new_from_file(LED0);
			gtk_grid_attach(GTK_GRID(led_matrix), image, i, j, 1, 1);			
		}
	}

	/* connect signals */
	gtk_builder_connect_signals(builder, NULL);

	g_signal_connect(start_button, "clicked", G_CALLBACK(on_launch_clicked), data);
	g_signal_connect(pause_button, "clicked", G_CALLBACK(on_pause_clicked), data);

	gtk_widget_set_sensitive(pause_button, FALSE); 

	/* display main_window */
	gtk_widget_show_all(main_window);

	gtk_main();

	g_free(data);

	return 0;
}
