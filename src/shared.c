#include "shared.h"

#include <gtk/gtk.h>

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ Utils                                                                ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

gint compare_strings(gconstpointer a, gconstpointer b) {
    return g_strcmp0((const char *)a, (const char *)b);
}

bool string_starts_with(const char *prefix, const char *str) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

struct TFileInfo *file_info_new(const char *name, bool is_dir) {
    struct TFileInfo *f = g_malloc(sizeof(*f));
    f->name = strdup(name);
    f->is_dir = is_dir;
    return f;
}

gint file_info_compare(gconstpointer f1, gconstpointer f2) {
    TFileInfo *_f1 = (TFileInfo *)f1;
    TFileInfo *_f2 = (TFileInfo *)f2;
    return compare_strings(_f1->name, (_f2->name));
}

// ╔╤══════════════════════════════════════════════════════════════════════╗
// ║│ External Resources                                                   ║
// ╚╧══════════════════════════════════════════════════════════════════════╝

GList *get_directory_contents(const char *path) {
    GFile *dir = g_file_new_for_path(path);
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
        if (name == NULL || string_starts_with(".", name)) continue;

        bool is_dir = g_file_info_get_file_type(info) == G_FILE_TYPE_DIRECTORY;
        TFileInfo *file = file_info_new(name, is_dir);
        file_list = g_list_append(file_list, file);
        g_object_unref(info);
    }

    g_file_enumerator_close(enumerator, NULL, NULL);
    g_object_unref(enumerator);
    g_object_unref(dir);

    file_list = g_list_sort(file_list, file_info_compare);

    return file_list;
}
