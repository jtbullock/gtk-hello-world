#include <gtk/gtk.h>

typedef struct TFileInfo {
    char *name;
    bool is_dir;
} TFileInfo;

struct TFileInfo *file_info_new(const char *name, bool is_dir);

gint file_info_compare(gconstpointer f1, gconstpointer f2);

gint compare_strings(gconstpointer a, gconstpointer b); 

bool string_starts_with(const char *prefix, const char *str);

GList *get_directory_contents(const char *path);
