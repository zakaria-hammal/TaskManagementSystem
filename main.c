#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "tasks.h"

GtkWidget *window;
GtkWidget *label[4];
GtkWidget *button[5];
GtkWidget *grid[6];
GtkWidget *retour[5];
GtkWidget *stack;
GtkEntryBuffer *buffer[3];
GtkWidget *entry[3];
GtkWidget *submit[5];
GtkWidget *dropdown[2];
GtkWidget *label1[7];
GtkWidget *label2[6];

Task* L;

static void Delete(GtkWidget *widget, gpointer user_data)
{
    char *string_id = g_strdup(gtk_entry_buffer_get_text(buffer[2]));
    char id[13];
    
    strcpy(id, string_id);

    if(DeleteTask(&L, id))
    {
        gtk_label_set_label(GTK_LABEL(label2[2]), "Task Id does not exists !!!");
        gtk_widget_add_css_class(GTK_WIDGET(label2[2]), "exist");
        gtk_widget_remove_css_class(GTK_WIDGET(label2[2]), "correct");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(label2[2]), "Task deleted successfully");
        gtk_widget_add_css_class(GTK_WIDGET(label2[2]), "correct");
        gtk_widget_remove_css_class(GTK_WIDGET(label2[2]), "exist");
    }
}

static void Insert(GtkWidget *widget, gpointer user_data)
{
    char *string_id = g_strdup(gtk_entry_buffer_get_text(buffer[0]));
    char id[13];

    strcpy(id, string_id);
    g_print("Id : %s\n", id);

    char *description = g_strdup(gtk_entry_buffer_get_text(buffer[1]));
    char desc[1001];
    
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
        gtk_widget_add_css_class(GTK_WIDGET(label1[5]), "exist");
        gtk_widget_remove_css_class(GTK_WIDGET(label1[5]), "correct");
        
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(label1[5]), "Task added successfully");
        gtk_widget_add_css_class(GTK_WIDGET(label1[5]), "correct");
        gtk_widget_remove_css_class(GTK_WIDGET(label1[5]), "exist");
    }
    
}

static void GoToDelete(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_delete");
}

static void GoToInsert(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_insert");
}

static void Home(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_home");
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[0]), "", 0);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[1]), "", 0);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[2]), "", 0);
    gtk_label_set_label(GTK_LABEL(label1[5]), "");
    gtk_label_set_label(GTK_LABEL(label2[2]), "");
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

    button[0] = gtk_button_new_with_mnemonic("Insert a Task");
    button[1] = gtk_button_new_with_mnemonic("Delete a Task");
    button[2] = gtk_button_new_with_mnemonic("Update a Task Status");
    button[3] = gtk_button_new_with_mnemonic("Display Tasks");
    button[4] = gtk_button_new_with_mnemonic("Search by Priority");

    label[0] = gtk_label_new (NULL);
    label[1] = gtk_label_new (NULL);
    label[2] = gtk_label_new (NULL);
    label[3] = gtk_label_new (NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, 
                                        "button { font-size: 20px; font-weight: normal; border-radius: 10px;} "
                                        "button:hover { font-size: 25px; font-weight: bold;} "
                                        ".bold-label { font-size: 25px; font-weight: bold;}"
                                        "entry { min-height: 50px; font-size: 20px; font-weight: 600; padding: 10px; border-radius: 10px;}"
                                        ".exist { color: red;}"
                                        ".correct { color: green;}");
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

    retour[0] = gtk_button_new_with_mnemonic("Retour");
    g_signal_connect (retour[0], "clicked", G_CALLBACK(Home), NULL);

    grid[1] = gtk_grid_new();
    
    buffer[0] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[0]), 12);

    entry[0] = gtk_entry_new_with_buffer(buffer[0]);

    buffer[1] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[1]), 12);

    entry[1] = gtk_entry_new_with_buffer(buffer[1]);

    const char *items1[] = {"1", "2", "3", "4", "5", NULL};

    dropdown[0] = gtk_drop_down_new_from_strings(items1);

    const char *items2[] = {"Pending", "In Progress", "Completed", NULL};

    dropdown[1] = gtk_drop_down_new_from_strings(items2);

    submit[0] = gtk_button_new_with_mnemonic("Submit");
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
    gtk_widget_add_css_class(GTK_WIDGET(label1[0]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label1[1]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label1[2]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label1[3]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label1[4]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label1[5]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label1[6]), "bold-label");

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
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(entry[0]), 1, 2, 3, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[2]), 1, 3, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(entry[1]), 1, 4, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[3]), 1, 5, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(dropdown[0]), 1, 6, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[4]), 1, 7, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(dropdown[1]), 1, 8, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[5]), 1, 9, 11, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(submit[0]), 1, 10, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(retour[0]), 6, 10, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[1]), GTK_WIDGET(label1[6]), 0, 11, 12, 1);

    grid[2] = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid[2]), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid[2]), 10);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid[2]), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid[2]), TRUE);
    g_signal_connect (button[1], "clicked", G_CALLBACK(GoToDelete), NULL);

    label2[0] = gtk_label_new("");
    label2[1] = gtk_label_new("");
    label2[2] = gtk_label_new("");
    label2[3] = gtk_label_new("");
    label2[4] = gtk_label_new("");
    label2[5] = gtk_label_new("");

    gtk_widget_add_css_class(GTK_WIDGET(label2[0]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[1]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[2]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[3]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[4]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[5]), "bold-label");

    gtk_label_set_label(GTK_LABEL(label2[1]), "Identifier (maximum 12 caracters):");
    gtk_label_set_xalign(GTK_LABEL(label2[1]), 0.0);

    buffer[2] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[2]), 12);

    entry[2] = gtk_entry_new_with_buffer(buffer[2]);

    submit[1] = gtk_button_new_with_mnemonic("Delete");
    retour[1] = gtk_button_new_with_mnemonic("Retour");

    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[0]), 0, 0, 12, 2);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[1]), 1, 2, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(entry[2]), 1, 3, 3, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[2]), 1, 4, 10, 2);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(submit[1]), 1, 6, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(retour[1]), 6, 6, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[3]), 0, 7, 12, 1);
    
    g_signal_connect (submit[1], "clicked", G_CALLBACK(Delete), NULL);
    g_signal_connect (retour[1], "clicked", G_CALLBACK(Home), NULL);

    gtk_stack_add_titled(GTK_STACK(stack), grid[0], "grid_home", "Home");
    gtk_stack_add_titled(GTK_STACK(stack), grid[1], "grid_insert", "Insert");
    gtk_stack_add_titled(GTK_STACK(stack), grid[2], "grid_delete", "Delete");

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