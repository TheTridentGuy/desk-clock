#include <gtk/gtk.h>
#include <time.h>
#include <glib/gprintf.h>

typedef struct {
    GtkWidget *time_label;
    GtkWidget *battery_label;
} Labels;

gboolean periodic(gpointer user_data) {
    Labels *labels = user_data;

    const time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time_format_string[] = "<span size=\"40pt\" weight=\"bold\" face=\"JetBrains Mono\">%H:%M:%S</span>";
    char time_string[strlen(time_format_string) + 1];
    strftime(time_string, strlen(time_format_string) + 1, time_format_string, &tm);
    gtk_label_set_markup(GTK_LABEL(labels->time_label), time_string);

    FILE *capacity_file = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (capacity_file) {
        char capacity[4];
        fgets(capacity, 4, capacity_file);
        capacity[strcspn(capacity, "\n")] = 0;
        char *battery_string = g_strdup_printf("<span size=\"15pt\" face=\"JetBrains Mono\">Battery: %s%</span>",
                                               capacity);
        gtk_label_set_markup(GTK_LABEL(labels->battery_label), battery_string);
    }
    fclose(capacity_file);
    return G_SOURCE_CONTINUE;
}

void activate(GtkApplication *app) {
    int box_margin = 5;
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Clock");

    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(container, box_margin);
    gtk_widget_set_margin_end(container, box_margin);
    gtk_widget_set_margin_top(container, box_margin);
    gtk_widget_set_margin_bottom(container, box_margin);
    gtk_window_set_child(GTK_WINDOW(window), container);

    GtkWidget *time_label = gtk_label_new("");
    GtkWidget *battery_label = gtk_label_new("");

    gtk_widget_set_halign(time_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(time_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(container), time_label);

    gtk_widget_set_halign(battery_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(battery_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(container), battery_label);
    gtk_window_present(GTK_WINDOW(window));

    Labels *labels = g_new(Labels, 1);
    labels->time_label = time_label;
    labels->battery_label = battery_label;
    periodic(labels);
    g_timeout_add(1000, periodic, labels);
}

int main(const int argc, char **argv) {
    GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
