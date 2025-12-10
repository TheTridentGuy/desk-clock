#include <gtk/gtk.h>
#include <time.h>

gboolean update_time(gpointer label) {
    const time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char fmt[] = "<span size=\"40pt\" weight=\"bold\" face=\"JetBrains Mono\">%H:%M:%S</span>";
    char time_str[sizeof(fmt)];
    strftime(time_str, sizeof(fmt), fmt, &tm);
    gtk_label_set_markup(GTK_LABEL(label), time_str);
    return G_SOURCE_CONTINUE;
}

void activate(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Clock");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    GtkWidget *label = gtk_label_new("");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), label);
    gtk_window_present(GTK_WINDOW(window));
    update_time(label);
    g_timeout_add(1000, update_time, label);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
