#include <gtk/gtk.h>

#include "shared.h"

static GtkWidget *folder_icon() {
    GtkWidget *picture = gtk_picture_new_for_filename("icon-folder.svg");
    //  gtk_widget_set_size_request(GTK_WIDGET(picture), 100, 100); // Optional:
    //  Set the desired size
    return picture;
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    TFileInfo *f = (TFileInfo *)data;
    g_print("Button {%s} clicked!\n", f->name);
}

static GtkWidget *directory_list_button(TFileInfo *f) {
    GtkWidget *button = gtk_button_new();
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *label = gtk_label_new(f->name);
    GtkWidget *icon_folder = folder_icon();

    if (f->is_dir) {
        gtk_box_append(GTK_BOX(button_box), folder_icon());
    }

    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_box_append(GTK_BOX(button_box), label);
    gtk_button_set_child(GTK_BUTTON(button), button_box);

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), f);

    return button;
}

static void add_file_list_item(gpointer file, gpointer box) {
    gtk_box_append(GTK_BOX(box), directory_list_button((TFileInfo *)file));
}

GtkWidget *directory_list(const char *path) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GList *dir_names = get_directory_contents(path);

    g_list_foreach(dir_names, add_file_list_item, box);
    //    g_clear_list(&dir_names,
    //                 (GDestroyNotify)g_free);  // TODO not the right place.
    //                 Also, is this the right way to free the TFileInfo struct?

    GtkWidget *sw = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw), box);

    return sw;
}
