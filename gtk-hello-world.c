#include <gtk/gtk.h>

static void print_hello (GtkWidget *widget, gpointer data)
{
  g_print ("Hello World\n");
}

static GtkWidget* build_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Hello");
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
  return window;
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window = build_window(app);
  GtkWidget *button;

  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  GtkWidget *image = gtk_image_new_from_file ("buppies.jpg");

  gtk_window_set_child (GTK_WINDOW (window), image);
  gtk_window_present (GTK_WINDOW (window));
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
