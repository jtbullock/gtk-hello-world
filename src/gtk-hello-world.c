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

static GtkWidget* build_video() {
  // Create a GtkVideo widget
    GtkWidget *video = gtk_video_new();
    const char *utf8Filename = "test-vid.mp4";
    gchar *glibEncodedFilename = g_filename_from_utf8(utf8Filename, -1, NULL,NULL,NULL);
      // Load a video file
    gtk_video_set_file(GTK_VIDEO(video), g_file_new_for_path(glibEncodedFilename));
    // start the video if visible
    gtk_video_set_autoplay(GTK_VIDEO(video),TRUE);
    return video;
}

static void set_reset_styles()
{
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "src/gtk-reset.css");

  gtk_style_context_add_provider_for_display(
      gdk_display_get_default(),
      GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_USER
  );

  g_object_unref(provider);
} 

static void set_custom_styles()
{
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "src/style.css");

  gtk_style_context_add_provider_for_display(
      gdk_display_get_default(),
      GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
  );

  g_object_unref(provider);
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window = build_window(app);
  GtkWidget *button;

  set_reset_styles();
  //set_custom_styles();

  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  GtkWidget *image = gtk_image_new_from_file ("assets/buppies.jpg");
  //GtkWidget *video = gtk_video_new_for_filename( "test-vid-2.mp4" );
  //GFile *file = g_file_new_for_path ("test-vid.mp4");
  //GtkWidget *video = gtk_video_new_for_file (file);
  //g_object_unref (file); // Release the GFile object
  GtkWidget *video = build_video();

  gtk_window_set_child (GTK_WINDOW (window), button);
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
