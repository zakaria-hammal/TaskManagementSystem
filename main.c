#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

GtkWidget *window;
GtkEntryBuffer *buffer;
GtkWidget *entry;
GtkWidget *label[4];
GtkWidget *button[5];
GtkWidget *grid[6];
GtkWidget *retour;
GtkWidget *stack;

static void Insert(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_insert");
}

static void Home(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_home");
}

static void on_activate(GtkApplication *app) 
{
    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 800);

    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);
    gtk_window_set_child(GTK_WINDOW(window), stack);

    grid[0] = gtk_grid_new ();
    
    gtk_grid_set_column_spacing(GTK_GRID(grid[0]), 20);
    gtk_grid_set_row_spacing(GTK_GRID(grid[0]), 20);

    gtk_grid_set_row_baseline_position(GTK_GRID(grid[0]), 0, GTK_BASELINE_POSITION_CENTER);
    gtk_grid_set_row_baseline_position(GTK_GRID(grid[0]), 1, GTK_BASELINE_POSITION_CENTER);
    gtk_grid_set_row_baseline_position(GTK_GRID(grid[0]), 2, GTK_BASELINE_POSITION_CENTER);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid[0]), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid[0]), TRUE);

    button[0] = gtk_button_new_with_label("Insert a Task");
    button[1] = gtk_button_new_with_label("Delete a Task");
    button[2] = gtk_button_new_with_label("Update a Task Status");
    button[3] = gtk_button_new_with_label("Display Tasks");
    button[4] = gtk_button_new_with_label("Search by Priority");

    buffer = gtk_entry_buffer_new(NULL, -1);
    entry = gtk_entry_new_with_buffer(buffer); 
    label[0] = gtk_label_new (NULL);
    label[1] = gtk_label_new (NULL);
    label[2] = gtk_label_new (NULL);
    label[3] = gtk_label_new (NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, "button { font-size: 25px; border-radius: 10px;} button:hover {font-weight: bold;}");

    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(label[0]), 0, 0, 22, 1);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(label[1]), 0, 0, 1, 14);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(label[2]), 0, 13, 22, 1);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(label[3]), 19, 0, 1, 14);

    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(button[0]), 1, 1, 10, 4);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(button[1]), 11, 1, 10, 4);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(button[2]), 1, 5, 10, 4);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(button[3]), 11, 5, 10, 4);
    gtk_grid_attach (GTK_GRID (grid[0]), GTK_WIDGET(button[4]), 1, 9, 20, 4);

    g_signal_connect (button[0], "clicked", G_CALLBACK(Insert), NULL);

    retour = gtk_button_new_with_label("Retour");

    grid[1] = gtk_grid_new();
    

    //gtk_grid_attach (GTK_GRID (grid[1]), GTK_WIDGET(retour), 0, 0, 1, 1);

    g_signal_connect (retour, "clicked", G_CALLBACK(Home), NULL);

    gtk_stack_add_titled(GTK_STACK(stack), grid[0], "grid_home", "Home");
    gtk_stack_add_titled(GTK_STACK(stack), grid[1], "grid_insert", "Insert");

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("stackof.holger.entry", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}