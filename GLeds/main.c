#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <gtk/gtk.h>

#include "gleds.h"
#include "oni_utils.h"
#include "patterns.h"
#include "utils.h"

static const char *LED_ARRAY[N_LEVELS] = { LED0, LED1, LED2, LED3 };
static gboolean running;
static gchar *mode;
static matrix_t *LED_MATRIX;

/* freq in Hertz */
static guint leds_freq = 20;
static guint matrix_freq = 20;

extern pattern_t patterns[];

/* 
	next_frame (n. ugly): tells the oni converter to convert the next frame,
 	we set the init value here 
*/
int next_frame = 1;

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

	running = FALSE;
}

gboolean set_matrix_values(gpointer user_data)
{
	static gchar *old_mode = "old";
	pattern_t *ptrn;

	if (strcmp(mode, old_mode) != 0) {
		reset_matrix(LED_MATRIX);
		g_print("new pattern: %s\n", mode);
		old_mode = g_strdup(mode);
	}

	for (ptrn = patterns; ptrn->desc; ptrn++) {
		if (strcmp(ptrn->desc, mode) == 0) {
			ptrn->func_ptr(LED_MATRIX);
		}
	}

	return running;
}

gboolean set_grid_values(gpointer user_data)
{
	GList *child;
	GtkWidget *grid = NULL;

	grid = GTK_WIDGET(user_data);
	child = gtk_container_get_children(GTK_CONTAINER(grid));

	int x, y;

	for (y = 1; y <= N_COLS; ++y) {
		for (x = 1; x <= N_ROWS; ++x) {
			GtkWidget *image = GTK_WIDGET(child->data);

			/* list begins at bottom-right corner of the matrix, then goes up and left... */
			gtk_image_set_from_file(GTK_IMAGE(image), LED_ARRAY[(LED_MATRIX->values)[N_ROWS-x][N_COLS-y]]);

			child = child->next;
		}
	}

	g_list_free(child);

	return running;
}

void on_combo_box_update(GtkWidget *widget, gpointer user_data)
{
	mode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
}

int freq_to_ms(guint freq)
{
	return ((double) 1 / freq) * 1000;
}

void on_start_clicked(GtkWidget *widget, gpointer user_data)
{
	running = TRUE;

	DataWrapper *data = (DataWrapper *) user_data;

	gtk_widget_set_sensitive(data->start_btn, FALSE); 
	gtk_widget_set_sensitive(data->pause_btn, TRUE); 

	g_timeout_add(freq_to_ms(leds_freq), set_grid_values, data->grid);
	g_timeout_add(freq_to_ms(matrix_freq), set_matrix_values, data->grid);

}

void on_launch_clicked(GtkWidget *widget, gpointer user_data)
{
	int s;

	running = FALSE;

	g_print("ok\n");
}

void on_spin_leds_update(GtkWidget *widget, gpointer user_data)
{
	leds_freq = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));
	running = FALSE;

	on_start_clicked(NULL, user_data);
}

void on_spin_matrix_update(GtkWidget *widget, gpointer user_data)
{
	matrix_freq = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));
	running = FALSE;

	on_start_clicked(NULL, user_data);
}

void on_window_destroy(GtkWidget *widget, gpointer user_data)
{
	
	quit();
}

int main(int argc, char **argv)
{
	gchar *filename = NULL;
	GError *error = NULL;
	GtkBuilder *builder = NULL;

	GtkWidget *led_matrix = NULL;
	GtkWidget *main_window = NULL;
	GtkWidget *pause_button = NULL;
	GtkWidget *start_button = NULL;
	GtkWidget *launch_button = NULL;
	GtkWidget *combo = NULL;
	GtkWidget *spinbutton_leds = NULL;
	GtkWidget *spinbutton_matrix = NULL;

	pattern_t *ptrn = NULL;

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
	launch_button = GTK_WIDGET(gtk_builder_get_object(builder, "launch_button"));

	spinbutton_leds = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_leds"));
	spinbutton_matrix = GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton_matrix"));

	/* ini data wrapper */
	DataWrapper *data = g_new(DataWrapper, 1);
	data->grid = led_matrix;
	data->start_btn = start_button;
	data->pause_btn = pause_button;

	/* init combo box */
	/*combo = gtk_combo_box_text_new();*/
	combo = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxtext1"));

	for (ptrn = patterns; ptrn->desc; ++ptrn) {
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), ptrn->desc);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

	/* init gui matrix */
	for (int i = 0; i < N_COLS; ++i) {
		for (int j = 0; j < N_ROWS; ++j) {
			GtkWidget *image;
			image = gtk_image_new_from_file(LED3);
			gtk_grid_attach(GTK_GRID(led_matrix), image, i, j, 1, 1);			
		}
	}

	/* connect signals */
	gtk_builder_connect_signals(builder, NULL);

	g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_clicked), data);
	g_signal_connect(pause_button, "clicked", G_CALLBACK(on_pause_clicked), data);

	g_signal_connect(launch_button, "clicked", G_CALLBACK(on_launch_clicked), NULL);
	g_signal_connect(combo, "changed", G_CALLBACK(on_combo_box_update), NULL);
	
	/* init mode value */
	on_combo_box_update(combo, NULL); 

	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_leds), leds_freq);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_matrix), matrix_freq);
	/*g_signal_connect(spinbutton_matrix, "value-changed", G_CALLBACK(on_spin_matrix_update), data);*/
	/*g_signal_connect(spinbutton_leds, "value-changed", G_CALLBACK(on_spin_leds_update), data);*/

	/* setup matrix */
	LED_MATRIX = init_matrix(N_ROWS, N_COLS);

	/* display main_window */
	gtk_widget_set_sensitive(pause_button, FALSE); 
	gtk_widget_show_all(main_window);

	gtk_main();

	g_free(data);
	g_free(mode);

	return 0;
}
