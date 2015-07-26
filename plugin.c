#include <geanyplugin.h>
#include <gtk/gtk.h>
#include <malloc.h>
#include "strlist.h"
#include "avrdude.h"

GeanyPlugin *geany_plugin;
GeanyData *geany_data;
GeanyFunctions *geany_functions;

PLUGIN_VERSION_CHECK(211);
PLUGIN_SET_INFO("AVR Manager", "TODO: write info text", "0.0", "Rasmus Antons <mail@rasmusantons.de>");

//char **isps, **avrs;
StrList *isps, *avrs;
int n_isps, n_avrs;

static GtkWidget *main_menu_item,
                 *settings_item,
                 *main_window,
                 *main_vbox,
                 *top_row,
                 *top_row_isp,
                 *top_row_avr,
                 *bottom_row,
                 *bottom_row_close,
                 *main_notebook,
                 *main_memory,
                 *main_fuses,
                 *main_makefile,
                 *main_settings,
                 *main_commands;

struct terminal {
	GtkWidget *scrolledwindow;
	GtkWidget *textbox;
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	GtkTextIter iter;
} terminal;

void print_to_terminal(char *text, int light)
{
	gtk_text_buffer_insert_with_tags_by_name(terminal.buffer, &terminal.iter, text, -1,
	                                         light ? "light" : "gray", "mono", NULL);
}

void println_to_terminal(char *text, int light)
{
	print_to_terminal(text, light);
	print_to_terminal("\n", light);
}

struct fuses {
	GtkWidget *dummy_1, *dummy_2, *dummy_3, *dummy_4, *dummy_5, *dummy_6;
} fuses;

GtkWidget *plugin_configure (GtkDialog *dialog)
{
	if (settings_item == NULL)
		settings_item = gtk_widget_new(GTK_TYPE_LABEL, "label", "Dummy Settings", 0.0, NULL);
	return settings_item;
}

void init_window()
{
	int i;
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(main_window), GTK_WINDOW(geany_data->main_widgets->window));
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 300, 200);
	gtk_window_set_title(GTK_WINDOW(main_window), "AVR Manager");
	
	//TOP
	main_vbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(main_window), main_vbox);
	
	top_row = gtk_hbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(main_vbox), top_row, FALSE, FALSE, 0);
	
	GtkWidget *isp_label = gtk_widget_new(GTK_TYPE_LABEL, "label", "ISP:", 0.0, NULL);
	gtk_box_pack_start(GTK_BOX(top_row), isp_label, FALSE, FALSE, 5);
	top_row_isp = gtk_combo_box_new_text();
	gtk_box_pack_start(GTK_BOX(top_row), top_row_isp, FALSE, FALSE, 0);
	
	GtkWidget *avr_label = gtk_widget_new(GTK_TYPE_LABEL, "label", "AVR:", 0.0, NULL);
	gtk_box_pack_start(GTK_BOX(top_row), avr_label, FALSE, FALSE, 5);
	top_row_avr = gtk_combo_box_new_text();
	gtk_box_pack_start(GTK_BOX(top_row), top_row_avr, FALSE, FALSE, 0);
	
	//BOTTOM
	bottom_row = gtk_hbox_new(FALSE, 1);
	gtk_box_pack_end(GTK_BOX(main_vbox), bottom_row, FALSE, FALSE, 0);
	
	GtkWidget *bottom_row_close = gtk_button_new_with_label("Close");
	gtk_widget_set_size_request(bottom_row_close, 70, 30);
	gtk_box_pack_end(GTK_BOX(bottom_row), bottom_row_close, FALSE, FALSE, 0);
	
	//MAIN
	main_notebook = gtk_notebook_new();
	gtk_box_pack_start(GTK_BOX(main_vbox), main_notebook, TRUE, TRUE, 0);
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(main_notebook), TRUE);
	
	//MEMORY
	main_memory = gtk_table_new(1, 1, TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), main_memory, gtk_label_new ("Memory"));
	
	//FUSES
	main_fuses = gtk_table_new(1, 1, TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), main_fuses, gtk_label_new ("Fuses"));
	
	//MAKE
	main_makefile = gtk_table_new(1, 1, TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), main_makefile, gtk_label_new ("Makefile"));
	
	//SETTINGS
	main_settings = gtk_table_new(1, 1, TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), main_settings, gtk_label_new ("Settings"));
	
	//COMMANDS
	main_commands = gtk_table_new(1, 1, TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), main_commands, gtk_label_new ("Commands"));
	
	terminal.scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(terminal.scrolledwindow),
	                               GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(main_commands), terminal.scrolledwindow);
	terminal.textbox = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(terminal.textbox), FALSE);
	GdkColor bgcolor;
	gdk_color_parse("#2E3436", &bgcolor);
	gtk_widget_modify_base(terminal.textbox, GTK_STATE_NORMAL, &bgcolor);
	terminal.buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(terminal.textbox));
	gtk_text_buffer_create_tag(terminal.buffer, "light", "foreground", "#EFEFEF", NULL);
	gtk_text_buffer_create_tag(terminal.buffer, "gray", "foreground", "#888888", NULL);
	gtk_text_buffer_create_tag (terminal.buffer, "mono", "family", "dejavu sans mono", NULL);
	gtk_text_buffer_get_iter_at_offset(terminal.buffer, &terminal.iter, 0);
	gtk_container_add(GTK_CONTAINER(terminal.scrolledwindow), terminal.textbox);
	
	gtk_widget_show_all(main_window);
}

void item_activate_cb(GtkMenuItem *menuitem, gpointer user_data)
{
	init_window();
	
	avrs = strlist_new();
	avrdude_get_avrs(avrs, &n_avrs);
	while (strlist_has_next(avrs))
		gtk_combo_box_append_text(GTK_COMBO_BOX(top_row_avr), strlist_get_next(avrs));
	isps = strlist_new();
	avrdude_get_isps(isps, &n_isps);
	while (strlist_has_next(isps))
		gtk_combo_box_append_text(GTK_COMBO_BOX(top_row_isp), strlist_get_next(isps));
}

void plugin_init(GeanyData *data)
{
	main_menu_item = gtk_menu_item_new_with_mnemonic("AVR Manager");
	gtk_widget_show(main_menu_item);
	gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), main_menu_item);
	g_signal_connect(main_menu_item, "activate", G_CALLBACK(item_activate_cb), NULL);
}

void plugin_cleanup(void)
{
	gtk_widget_destroy(main_menu_item);
	//TODO: remove menu entry
}
