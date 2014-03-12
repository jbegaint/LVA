#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#define GLADE_FILE "Gleds.glade"

#define LED0 "led_full.png"
#define LED1 "led_1.png"
#define LED2 "led_2.png"
#define LED3 "led_black.png"

#define N_ROWS 7
#define N_COLS 5

#define N_LEVELS 4

static const char *LED_ARRAY[N_LEVELS] = { LED0, LED1, LED2, LED3 };

static int PINS_LEVELS[7][5];

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
			gtk_image_set_from_file(GTK_IMAGE(image), LED_ARRAY[rand() % N_LEVELS]);
		}
	}

	g_list_free(children);
	g_free(led_state);

	return continue_loop;
}

void reset_matrix(void)
{
	int i, j;

	for (i = 0; i < N_ROWS; i++) {
		for (j = 0; j < N_COLS; j++) {
			PINS_LEVELS[i][j] = 3;
		}		
	}
}

void toggle_pin(int i, int j)
{
	if (PINS_LEVELS[i][j] == 0) {
		PINS_LEVELS[i][j] = 3;
	}
	else {
		PINS_LEVELS[i][j] = 0;
	}
}

gboolean led_by_led(gpointer user_data)
{
	static int led_x = 0;
	static int led_y = 0;

	int i, j;

	for (i = 0; i < N_ROWS; i++) {
		for (j = 0; j < N_COLS; j++) {
			if (i == led_y && j == led_x) {
				toggle_pin(i, j);
			}
		}		
	}

	if ((++led_x) == N_COLS) {
		led_x = 0;
		led_y = (++led_y) % N_ROWS;
	}

	return continue_loop;
}

gboolean set_pin_values(gpointer user_data)
{
	GList *children, *l;
	GtkWidget *grid = NULL;

	grid = GTK_WIDGET(user_data);
	children = gtk_container_get_children(GTK_CONTAINER(grid));

	int x, y;

	for (y = 1; y <= N_COLS; ++y) {
		for (x = 1; x <= N_ROWS; ++x) {
			GtkWidget *image = children->data;

			/* list begins at bottom-right corner of the matrix... */
			gtk_image_set_from_file(GTK_IMAGE(image), LED_ARRAY[PINS_LEVELS[N_ROWS-x][N_COLS-y]]);

			children = children->next;
		}
	}

	g_list_free(children);

	return continue_loop;
}

void on_launch_clicked(GtkWidget *widget, gpointer user_data)
{
	continue_loop = TRUE;

	DataWrapper *data = (DataWrapper *) user_data;

	gtk_widget_set_sensitive(data->start_btn, FALSE); 
	gtk_widget_set_sensitive(data->pause_btn, TRUE); 

	g_timeout_add(33, set_pin_values, data->grid);
	g_timeout_add(100, led_by_led, data->grid);

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
			image = gtk_image_new_from_file(LED3);
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
	
	/* clean default values (just to be sure) */
	reset_matrix();

	gtk_main();

	g_free(data);

	return 0;
}
