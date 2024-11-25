#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "tasks.h"

GtkWidget *window;
GtkWidget *label[4];
GtkWidget *button[5];
GtkWidget *grid[6];
GtkWidget *retour;
GtkWidget *stack;
GtkEntryBuffer *buffer[2];
GtkWidget *entry[2];
GtkWidget *submit[5];
GtkWidget *dropdown[2];
GtkWidget *label1[7];

Task* L;

static void Insert(GtkWidget *widget, gpointer user_data)
{
    char *string_id = g_strdup(gtk_entry_buffer_get_text(buffer[0]));
    char id[13];
    id[0] = '\0';

    for (int i = 0; i < 10; i++)
    {
        id[i] = string_id[i];
        if(id[i] == '\0')
        {
            break;
        }
    }
    g_print("Id : %s\n", id);
    char *description = g_strdup(gtk_entry_buffer_get_text(buffer[1]));
    char desc[1001];
    desc[0] = '\0';

    for (int i = 0; i < 1001; i++)
    {
        desc[i] = description[i];
        if(desc[i] == '\0')
        {
            break;
        }
    }

    strcpy(desc, description);

    GtkStringObject *selected_item1 = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown[0]));

    int choice = atoi(gtk_string_object_get_string(selected_item1));
    g_print("Selected: %d\n", choice);
    
    GtkStringObject *selected_item2 = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown[1]));
    
    const char* selected_text = gtk_string_object_get_string(selected_item2);
    char status[15];
    strcpy(status, selected_text);

    g_print("Selected: %s\n", selected_text);

    if(InsertTask(&L, id, description, choice, status))
    {
        gtk_label_set_label(GTK_LABEL(label1[5]), "Task Id Already exists !!!");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(label1[5]), "Task added successfully");
    }
    
}

static void GoToInsert(GtkWidget *widget, gpointer user_data)
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
    gtk_window_set_default_size (GTK_WINDOW (window), 800, 900);

    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);
    gtk_window_set_child(GTK_WINDOW(window), stack);

    grid[0] = gtk_grid_new ();
    
    gtk_grid_set_column_spacing(GTK_GRID(grid[0]), 20);
    gtk_grid_set_row_spacing(GTK_GRID(grid[0]), 20);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid[0]), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid[0]), TRUE);

    button[0] = gtk_button_new_with_label("Insert a Task");
    button[1] = gtk_button_new_with_label("Delete a Task");
    button[2] = gtk_button_new_with_label("Update a Task Status");
    button[3] = gtk_button_new_with_label("Display Tasks");
    button[4] = gtk_button_new_with_label("Search by Priority");

    label[0] = gtk_label_new (NULL);
    label[1] = gtk_label_new (NULL);
    label[2] = gtk_label_new (NULL);
    label[3] = gtk_label_new (NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, "button { font-size: 20px; font-weight: normal; border-radius: 10px;} button:hover { font-size: 25px; font-weight: bold;} label {font-size: 25px; font-weight: bold; border-radius: 5px;} entry {font-size: 15px; font-weight: bold; border-radius: 10px;} ");

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

    g_signal_connect (button[0], "clicked", G_CALLBACK(GoToInsert), NULL);

    retour = gtk_button_new_with_label("Retour");
    g_signal_connect (retour, "clicked", G_CALLBACK(Home), NULL);

    grid[1] = gtk_grid_new();
    
    buffer[0] = gtk_entry_buffer_new(NULL, -1);

    entry[0] = gtk_entry_new_with_buffer(buffer[0]);

    buffer[1] = gtk_entry_buffer_new(NULL, -1);

    entry[1] = gtk_entry_new_with_buffer(buffer[1]);

    const char *items1[] = {"1", "2", "3", "4", "5", NULL};

    dropdown[0] = gtk_drop_down_new_from_strings(items1);

    const char *items2[] = {"Pending", "In Progress", "Completed", NULL};

    dropdown[1] = gtk_drop_down_new_from_strings(items2);

    submit[0] = gtk_button_new_with_label("Submit");
    g_signal_connect (submit[0], "clicked", G_CALLBACK(Insert), NULL);

    gtk_grid_set_column_spacing(GTK_GRID(grid[1]), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid[1]), 10);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid[1]), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid[1]), TRUE);
    
    label1[0] = gtk_label_new("");
    label1[1] = gtk_label_new("");
    label1[2] = gtk_label_new("");
    label1[3] = gtk_label_new("");
    label1[4] = gtk_label_new("");
    label1[5] = gtk_label_new("");
    label1[6] = gtk_label_new("");

    gtk_label_set_label(GTK_LABEL(label1[1]), "Identifier (maximum 12 caracters):");
    gtk_label_set_xalign(GTK_LABEL(label1[1]), 0.0);
    gtk_label_set_label(GTK_LABEL(label1[2]), "Description (maximum 1000 caracters):");
    gtk_label_set_xalign(GTK_LABEL(label1[2]), 0.0);
    gtk_label_set_label(GTK_LABEL(label1[3]), "Priority Level :");
    gtk_label_set_xalign(GTK_LABEL(label1[3]), 0.0);
    gtk_label_set_label(GTK_LABEL(label1[4]), "Status :");
    gtk_label_set_xalign(GTK_LABEL(label1[4]), 0.0);
    

    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[0]), 0, 0, 12, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[1]), 1, 1, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(entry[0]), 1, 2, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[2]), 1, 3, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(entry[1]), 1, 4, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[3]), 1, 5, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(dropdown[0]), 1, 6, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[4]), 1, 7, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(dropdown[1]), 1, 8, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[5]), 0, 9, 11, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(submit[0]), 1, 10, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(retour), 6, 10, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[6]), 0, 11, 12, 1);

    gtk_stack_add_titled(GTK_STACK(stack), grid[0], "grid_home", "Home");
    gtk_stack_add_titled(GTK_STACK(stack), grid[1], "grid_insert", "Insert");

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[])
{
    GtkApplication *app;
    int status;
    L = NULL;

    app = gtk_application_new ("stackof.holger.entry", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}