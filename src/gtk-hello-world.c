#include <dirent.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "glib-object.h"
#include <glib.h>
#include <gio/gio.h>

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ Utils                                                                ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

static gint compare_strings(gconstpointer a, gconstpointer b) {
    return g_strcmp0((const char *)a, (const char *)b);
}

bool string_starts_with(const char *prefix, const char *str) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ App Styling Setup                                                    ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

static GtkCssProvider *reset_style_provider = NULL;
static GtkCssProvider *custom_style_provider = NULL;

static void set_reset_styles() {
    if (reset_style_provider == NULL) {
        reset_style_provider = gtk_css_provider_new();
        gtk_style_context_add_provider_for_display(
            gdk_display_get_default(), GTK_STYLE_PROVIDER(reset_style_provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    gtk_css_provider_load_from_path(reset_style_provider, "src/gtk-reset.css");
}

static void set_custom_styles() {
    if (custom_style_provider == NULL) {
        custom_style_provider = gtk_css_provider_new();
        gtk_style_context_add_provider_for_display(
            gdk_display_get_default(),
            GTK_STYLE_PROVIDER(custom_style_provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    gtk_css_provider_load_from_path(custom_style_provider, "src/style.css");
}

static void set_css_styles() {
    set_reset_styles();
    set_custom_styles();
}

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ Keyboard handling                                                    ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval,
                             guint keycode, GdkModifierType state,
                             gpointer user_data) {
    static guint sequence[3] = {0};  // Store last 3 keyvals
    static int index = 0;

    // Store the key
    sequence[index] = keyval;
    index = (index + 1) % 3;  // Circular buffer

    // Example sequence: Ctrl+Shift+X
    if ((state & GDK_CONTROL_MASK) && (state & GDK_SHIFT_MASK) &&
        keyval == GDK_KEY_R) {
        set_css_styles();
        g_print("Ctrl+Shift+R pressed, setting css styles.\n");
    }

    // Example sequence: Up, Up, Down
    // if (sequence[0] == GDK_KEY_Up &&
    //    sequence[1] == GDK_KEY_Up &&
    //    sequence[2] == GDK_KEY_Down) {
    //    g_print("Up, Up, Down sequence detected!\n");
    //}

    return GDK_EVENT_PROPAGATE;  // Pass event on to other handlers
}

static void init_key_monitoring(GtkWidget *window) {
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_press),
                     NULL);
    gtk_widget_add_controller(window, key_controller);
}

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ External Resources                                                   ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

typedef struct TFileInfo {
    char *name;
    bool is_dir;
} TFileInfo;

struct TFileInfo *file_info_new(const char *name, bool is_dir) {
    struct TFileInfo *f = g_malloc(sizeof(*f));
    f->name = strdup(name);
    f->is_dir = is_dir;
    return f;
}

static GList *get_home_contents() {
    GFile *dir = g_file_new_for_path("/");
    const char *attributes =
        G_FILE_ATTRIBUTE_STANDARD_NAME "," G_FILE_ATTRIBUTE_STANDARD_TYPE;
    GFileEnumerator *enumerator = g_file_enumerate_children(
        dir, attributes, G_FILE_QUERY_INFO_NONE, NULL, NULL);

    GList *file_list = NULL;
    GFileInfo *info;

    while (true) {
        info = g_file_enumerator_next_file(enumerator, NULL, NULL);
        if (info == NULL) break;

        const char *name = g_file_info_get_name(info);
        if (name == NULL || string_starts_with(".", name)) break;
        
        bool is_dir = g_file_info_get_file_type(info) == G_FILE_TYPE_DIRECTORY;
        TFileInfo *file = file_info_new(name, is_dir);
        file_list = g_list_append(file_list, file);
        g_object_unref(info);
    }

    g_file_enumerator_close(enumerator, NULL, NULL);
    g_object_unref(enumerator);
    g_object_unref(dir);

    // string_list = g_list_sort(string_list, compare_strings);
    // 
    return file_list;


    // return string_list;
}

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ Debugging/Console                                                    ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

static void print_hello() { g_print("Hello World\n"); }

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ Widgets                                                              ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

static GtkWidget *build_window(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Hello");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    return window;
}

static GtkWidget *build_video() {
    // Create a GtkVideo widget
    GtkWidget *video = gtk_video_new();
    const char *utf8Filename = "test-vid.mp4";
    gchar *glibEncodedFilename =
        g_filename_from_utf8(utf8Filename, -1, NULL, NULL, NULL);
    // Load a video file
    gtk_video_set_file(GTK_VIDEO(video),
                       g_file_new_for_path(glibEncodedFilename));
    // start the video if visible
    gtk_video_set_autoplay(GTK_VIDEO(video), TRUE);
    return video;
}

static void on_label_clicked(GtkGestureClick *gesture, int n_press, double x,
                             double y, gpointer user_data) {
    g_print("Label clicked at (%.0f x %.0f) with %d presses\n", x, y, n_press);
}

static GtkWidget *folder_icon() {
    GtkWidget *picture = gtk_picture_new_for_filename("icon-folder.svg");
    //  gtk_widget_set_size_request(GTK_WIDGET(picture), 100, 100); // Optional:
    //  Set the desired size
    return picture;
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
    return button;
}

static void add_file_list_item(gpointer file, gpointer box) {
    gtk_box_append(GTK_BOX(box), directory_list_button((TFileInfo *)file));
}

static GtkWidget *directory_list() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GList *dir_names = get_home_contents();

    g_list_foreach(dir_names, add_file_list_item, box);
    g_clear_list(&dir_names, (GDestroyNotify)g_free);

    GtkWidget *sw = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw), box);

    return sw;
}

static void video_player() {
    // GtkWidget *video = gtk_video_new_for_filename( "test-vid-2.mp4" );
    // GFile *file = g_file_new_for_path ("test-vid.mp4");
    // GtkWidget *video = gtk_video_new_for_file (file);
    // g_object_unref (file); // Release the GFile object
    // GtkWidget *video = build_video();
}

// ╔╤◇════════════════════════════════════════════════════════════════════◇╗
// ╚╧◇════════════════════════════════════════════════════════════════════◇╝

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = build_window(app);
    GtkWidget *button;

    set_css_styles();

    init_key_monitoring(window);

    button = gtk_button_new_with_label("Hello World");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    GtkWidget *image = gtk_image_new_from_file("assets/buppies.jpg");
    gtk_window_set_child(GTK_WINDOW(window), directory_list());
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
