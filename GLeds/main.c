#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "../c_gpio/patterns.h"

#define GLADE_FILE "Gleds.glade"

#define LED0 "led_full.png"
#define LED1 "led_1.png"
#define LED2 "led_2.png"
#define LED3 "led_black.png"

static GtkWidget *combo = NULL;
static const char *LED_ARRAY[N_LEVELS] = { LED0, LED1, LED2, LED3 };
static gboolean continue_loop;
static int PATTERN = LED_BY_LED;
static matrix_t LED_MATRIX;

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

gboolean set_matrix_values(gpointer user_data)
{
	gchar *mode;
	static gchar *old_mode = "qqqq";

	mode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));

	if (mode) {

		if (strcmp(mode, old_mode) != 0) {
			reset_matrix(&LED_MATRIX);
			old_mode = g_strdup(mode);
		}

		if (strcmp("led by led", mode) == 0) {
			set_pattern_led_by_led(&LED_MATRIX);
		}
		else if (strcmp("led by led toggle", mode) == 0) {
			set_pattern_led_by_led_toggle(&LED_MATRIX);
		}
		else if (strcmp("random", mode) == 0) {
			set_pattern_random(&LED_MATRIX);
		}
	}
	else {
		set_pattern_led_by_led(&LED_MATRIX);
	}

	/* free old_mode ? */
	g_free(mode);

	return continue_loop;
}

gboolean set_grid_values(gpointer user_data)
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
			gtk_image_set_from_file(GTK_IMAGE(image), LED_ARRAY[(LED_MATRIX.values)[N_ROWS-x][N_COLS-y]]);

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

	g_timeout_add(33, set_grid_values, data->grid);
	g_timeout_add(50, set_matrix_values, data->grid);

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
	GtkWidget *vbox = NULL;

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

	combo = gtk_combo_box_text_new();

	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "led by led");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "led by led toggle");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "random");

	vbox = GTK_WIDGET(gtk_builder_get_object(builder, "vbox"));
	gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 10);

	/* init gui matrix */
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
	
	/* setup matrix */
	setup_matrix(&LED_MATRIX, N_ROWS, N_COLS);

	gtk_main();

	g_free(data);

	return 0;
}
