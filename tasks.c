#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "tasks.h"

int InsertTask(Task **L, char id[13], char description[1001], int priorityLevel, char status[15])
{
    if (L == NULL) {
        return 1;
    }

    Task *Q = *L;
    while (Q != NULL) {
        if (!strcmp(Q->Id, id)) {
            return 1;
        }
        Q = Q->next;
    }

    Task *P = malloc(sizeof(Task));
    if (P == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    strcpy(P->Id, id);
    strcpy(P->Description, description);
    strcpy(P->Status, status);
    P->PriorityLevel = priorityLevel;
    P->next = NULL;

    if (*L == NULL || (*L)->PriorityLevel < priorityLevel) {
        P->next = *L;
        *L = P;
        return 0;
    }

    Q = *L;
    while (Q->next != NULL && Q->next->PriorityLevel >= priorityLevel) {
        Q = Q->next;
    }

    P->next = Q->next;
    Q->next = P;
    return 0;
}


int DeleteTask(Task **L, char id[13])
{
    if(L != NULL)
    {
        if(*L == NULL)
        {
            return 1;
        }

        Task *P;

        if(!strcmp((*L)->Id, id))
        {
            P = *L;
            *L = (*L)->next;
            free(P);

            return 0;
        }

        Task *Q = *L;

        while(Q->next != NULL && strcmp(Q->next->Id, id) != 0)
        {
            Q = Q->next;
        }

        if(Q->next == NULL)
        {
            return 1;
        }

        P = Q->next;
        Q->next = P->next;
        free(P);

        return 0;
    }

    return 1;
}

int UpdateTaskStatus(Task **L, char id[13], char status[15])
{
    if(L != NULL)
    {
        if(*L == NULL)
        {
            return 1;
        }

        Task *Q = *L;

        while (Q != NULL && strcmp(Q->Id, id) != 0)
        {
            Q = Q->next;
        }
        

        if(Q == NULL)
        {
            return 1;
        }

        strcpy(Q->Status, status);

        return 0;
    }

    return 1;
}

void BuildList(Task **L, Task **L1, Task **L2, Task **L3)
{
    if(L1 != NULL && L2 != NULL && L3 != NULL && L != NULL)
    {
        
        *L1 = NULL;
        *L2 = NULL;
        *L3 = NULL;
    
        if(*L != NULL)
        {
            Task *Q = *L;
            Task *P;

            while(Q != NULL)
            {
                P = malloc(sizeof(Task));
                P->PriorityLevel = Q->PriorityLevel;
                strcpy(P->Description, Q->Description);
                strcpy(P->Status, Q->Status);
                strcpy(P->Id, Q->Id);
                if (!strcmp(Q->Status, "Pending"))
                {
                    P->next = *L1;
                    *L1 = P;
                }
                else if (!strcmp(Q->Status, "In Progress"))
                {
                    P->next = *L2;
                    *L2 = P;
                }
                else
                {
                    P->next = *L3;
                    *L3 = P;
                }

                Q = Q->next;
                
            }
        }
        
    }
}

void DestroyList(Task **L)
{
    if(L != NULL)
    {
        Task *Q;
        while (*L != NULL)
        {
            Q = *L;
            *L = (*L)->next;
            free(Q);
        }
        
    }
}

void BuildSingleList(Task **L, Task **L1, int priorityLevel)
{
    if(L != NULL)
    {
        *L1 = NULL;
        if(priorityLevel >= 1 && priorityLevel <= 5)
        {
            Task *Q = *L;
            Task *P;

            while (Q != NULL)
            {
                if(Q->PriorityLevel == priorityLevel)
                {
                    P = malloc(sizeof(Task));
                    P->PriorityLevel = Q->PriorityLevel;
                    strcpy(P->Description, Q->Description);
                    strcpy(P->Status, Q->Status);
                    strcpy(P->Id, Q->Id);
                    P->next = *L1;
                    *L1 = P;
                }
                
                Q = Q->next;
            }
            
        }
        
    }
}

GtkWidget *window;
GtkWidget *label[4];
GtkWidget *button[5];
GtkWidget *grid[6];
GtkWidget *retour[5];
GtkWidget *stack;
GtkEntryBuffer *buffer[4];
GtkWidget *entry[4];
GtkWidget *submit[4];
GtkWidget *dropdown[4];
GtkWidget *label1[7];
GtkWidget *label2[4];
GtkWidget *label3[5];
GtkWidget *scrolled_window1;
GtkWidget *scrolled_window2;
GtkWidget **label4;
GtkWidget **pendingLabel;
GtkWidget **inProgressLabel;
GtkWidget **completedLabel;
GtkWidget *box;
GtkWidget *box1;
int size[4];
GtkWidget *spaceLabel1;
GtkWidget **spaceLabel2;
GtkWidget *headLabel;
GtkWidget *titleLabel;
GtkWidget **infoLabel;
GtkWidget **bottomLabel;

Task *L;
Task *L1;
Task *L2;
Task *L3;
Task *L4;

FILE *file;

static void on_app_shutdown(GApplication *app, gpointer user_data) 
{
    DestroyList(&L1);
    DestroyList(&L2);
    DestroyList(&L3);
    DestroyList(&L4);

    Task *task;
    FileTask *fileTask = malloc(sizeof(FileTask));

    file = fopen("tasks.dat", "wb");

    while(L != NULL)
    {
        task = L;
        strcpy(fileTask->Id, task->Id);
        strcpy(fileTask->Description, task->Description);
        strcpy(fileTask->Status, task->Status);
        fileTask->PriorityLevel = task->PriorityLevel;
        L = L->next;
        fwrite(fileTask, sizeof(FileTask), 1, file);
        free(task);
    }

    free(fileTask);
    fclose(file);
}

static void Search(GtkWidget *widget, gpointer user_data)
{
    if(infoLabel != NULL)
    {
        for (int i = 0; i < size[3]; i++)
        {
            gtk_box_remove(GTK_BOX(box1), GTK_WIDGET(infoLabel[i]));
        }

        free(infoLabel);
        infoLabel = NULL;
    }

    if(bottomLabel != NULL)
    {
        gtk_box_remove(GTK_BOX(box1), GTK_WIDGET(*bottomLabel));
        free(bottomLabel);
        bottomLabel = NULL;
    }

    DestroyList(&L4);
    L4 = NULL;

    GtkStringObject *selected_item = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown[3]));

    int choice = atoi(gtk_string_object_get_string(selected_item));
    BuildSingleList(&L, &L4, choice);

    if(L4 == NULL)
    {
        size[3] = 1;
        infoLabel = malloc(sizeof(GtkWidget*));
        bottomLabel = malloc(sizeof(GtkWidget*));
        *bottomLabel = gtk_label_new_with_mnemonic("\n");
        *infoLabel = gtk_label_new_with_mnemonic("No Task Found");
        gtk_widget_add_css_class(GTK_WIDGET(*infoLabel), "bold-label");
        gtk_label_set_xalign(GTK_LABEL(*infoLabel), 0.0);
        gtk_box_append(GTK_BOX(box1), GTK_WIDGET(*infoLabel));
        gtk_box_append(GTK_BOX(box1), GTK_WIDGET(*bottomLabel));
        g_print("Zero\n");
        return;
    }

    Task *P = L4;
    size[3] = 0;

    while(P != NULL)
    {
        size[3] += 1;
        P = P->next;
    }

    infoLabel = malloc(size[3] * sizeof(GtkWidget*));
    bottomLabel = malloc(sizeof(GtkWidget*));
    *bottomLabel = gtk_label_new_with_mnemonic("\n");
    char stat[11002] = "";

    P = L4;
    for (int i = 0; i < size[3]; i++)
    {
        g_print("%s\n", P->Id);
        strcpy(stat, "Id : ");
        strcat(stat, P->Id);
        strcat(stat, "\nDescription : ");
        strcat(stat, P->Description);
        strcat(stat, "\nStatus : ");
        strcat(stat, P->Status);
        infoLabel[i] = gtk_label_new_with_mnemonic(stat);
        gtk_widget_add_css_class(GTK_WIDGET(infoLabel[i]), "bold-label");
        gtk_label_set_xalign(GTK_LABEL(infoLabel[i]), 0.0);
        gtk_box_append(GTK_BOX(box1), GTK_WIDGET(infoLabel[i]));
        P = P->next;
    }

    gtk_box_append(GTK_BOX(box1), GTK_WIDGET(*bottomLabel));
}

static void Update(GtkWidget *widget, gpointer user_data)
{
    char *string_id = g_strdup(gtk_entry_buffer_get_text(buffer[3]));
    char id[13] = "";
    
    strcpy(id, string_id);

    GtkStringObject *selected_item = gtk_drop_down_get_selected_item(GTK_DROP_DOWN(dropdown[2]));
    
    const char* selected_text = gtk_string_object_get_string(selected_item);
    char status[15];
    strcpy(status, selected_text);

    if(UpdateTaskStatus(&L, id, status))
    {
        gtk_label_set_label(GTK_LABEL(label3[3]), "Task Id does not exists !!!");
        gtk_widget_add_css_class(GTK_WIDGET(label3[3]), "exist");
        gtk_widget_remove_css_class(GTK_WIDGET(label3[3]), "correct");
    }
    else
    {
        gtk_label_set_label(GTK_LABEL(label3[3]), "Task Status Updated successfully");
        gtk_widget_add_css_class(GTK_WIDGET(label3[3]), "correct");
        gtk_widget_remove_css_class(GTK_WIDGET(label3[3]), "exist");
    }

    Task *Q = L;

    while(Q != NULL)
    {
        g_print("%s\n", Q->Status);
        Q = Q->next;
    }

    g_free(string_id);
}

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

    g_free(string_id);
}

static void Insert(GtkWidget *widget, gpointer user_data)
{
    char *string_id = g_strdup(gtk_entry_buffer_get_text(buffer[0]));
    char id[13] = "";

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

    g_print("Selected: %s\n", status);

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

    g_free(string_id);
    g_free(description);
}

static void GoToSearch(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_search");
}

static void GoToDisplay(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_display");
    BuildList(&L, &L1, &L2, &L3);

    Task *P;
    char stat[11002] = "";

    size[0] = 0;
    size[1] = 0;
    size[2] = 0;

    label4 = malloc(3 * sizeof(GtkWidget*));
    label4[0] = gtk_label_new_with_mnemonic("Pending");
    gtk_widget_add_css_class(GTK_WIDGET(label4[0]), "title-label");
    label4[1] = gtk_label_new_with_mnemonic("\nIn Progress");
    gtk_widget_add_css_class(GTK_WIDGET(label4[1]), "title-label");
    label4[2] = gtk_label_new_with_mnemonic("\nCompleted");
    gtk_widget_add_css_class(GTK_WIDGET(label4[2]), "title-label");

    P = L1;
    while (P != NULL)
    {
        size[0] += 1;
        P = P->next;
    }

    P = L2;
    while (P != NULL)
    {
        size[1] += 1;
        P = P->next;
    }

    P = L3;
    while (P != NULL)
    {
        size[2] += 1;
        P = P->next;
    }

    pendingLabel = malloc(size[0] * sizeof(GtkWidget*));
    inProgressLabel = malloc(size[1] * sizeof(GtkWidget*));
    completedLabel = malloc(size[2] * sizeof(GtkWidget*));
    spaceLabel2 = malloc(sizeof(GtkWidget*));
    *spaceLabel2 = gtk_label_new_with_mnemonic("\n");
    char pr[3];

    gtk_box_append(GTK_BOX(box), GTK_WIDGET(label4[0]));

    P = L1;
    for (int i = 0; i < size[0]; i++)
    {
        g_print("%s\n", P->Id);
        strcpy(stat, "Id : ");
        strcat(stat, P->Id);
        strcat(stat, "\nDescription : ");
        strcat(stat, P->Description);
        strcat(stat, "\nPriority Level : ");
        sprintf(pr, "%d", P->PriorityLevel);
        strcat(stat, pr);
        pendingLabel[i] = gtk_label_new_with_mnemonic(stat);
        gtk_widget_add_css_class(GTK_WIDGET(pendingLabel[i]), "bold-label");
        gtk_label_set_xalign(GTK_LABEL(pendingLabel[i]), 0.0);
        gtk_box_append(GTK_BOX(box), GTK_WIDGET(pendingLabel[i]));
        P = P->next;
    }

    gtk_box_append(GTK_BOX(box), GTK_WIDGET(label4[1]));
    P = L2;
    for (int i = 0; i < size[1]; i++)
    {
        g_print("%s\n", P->Id);
        strcpy(stat, "Id : ");
        strcat(stat, P->Id);
        strcat(stat, "\nDescription : ");
        strcat(stat, P->Description);
        strcat(stat, "\nPriority Level : ");
        sprintf(pr, "%d", P->PriorityLevel);
        strcat(stat, pr);
        inProgressLabel[i] = gtk_label_new_with_mnemonic(stat);
        gtk_widget_add_css_class(GTK_WIDGET(inProgressLabel[i]), "bold-label");
        gtk_label_set_xalign(GTK_LABEL(inProgressLabel[i]), 0.0);
        gtk_box_append(GTK_BOX(box), GTK_WIDGET(inProgressLabel[i]));
        P = P->next;
    }

    gtk_box_append(GTK_BOX(box), GTK_WIDGET(label4[2]));
    P = L3;
    for (int i = 0; i < size[2]; i++)
    {
        g_print("%s\n", P->Id);
        strcpy(stat, "Id : ");
        strcat(stat, P->Id);
        strcat(stat, "\nDescription : ");
        strcat(stat, P->Description);
        strcat(stat, "\nPriority Level : ");
        sprintf(pr, "%d", P->PriorityLevel);
        strcat(stat, pr);
        completedLabel[i] = gtk_label_new_with_mnemonic(stat);
        gtk_widget_add_css_class(GTK_WIDGET(completedLabel[i]), "bold-label");
        gtk_label_set_xalign(GTK_LABEL(completedLabel[i]), 0.0);
        gtk_box_append(GTK_BOX(box), GTK_WIDGET(completedLabel[i]));
        P = P->next;
    }


    gtk_box_append(GTK_BOX(box), GTK_WIDGET(*spaceLabel2));
}

static void GoToUpdate(GtkWidget *widget, gpointer user_data)
{
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "grid_update");
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
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[3]), "", 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[0]), 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[1]), 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[2]), 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[3]), 0);    
    gtk_label_set_label(GTK_LABEL(label1[5]), "");
    gtk_label_set_label(GTK_LABEL(label2[2]), "");
    gtk_label_set_label(GTK_LABEL(label3[3]), "");

    if(label4 != NULL)
    {
        gtk_box_remove(GTK_BOX(box), GTK_WIDGET(label4[0]));
        gtk_box_remove(GTK_BOX(box), GTK_WIDGET(label4[1]));
        gtk_box_remove(GTK_BOX(box), GTK_WIDGET(label4[2]));

        free(label4);
        label4 = NULL;
    }

    if(pendingLabel != NULL)
    {
        for (int i = 0; i < size[0]; i++)
        {
            gtk_box_remove(GTK_BOX(box), GTK_WIDGET(pendingLabel[i]));
        }

        free(pendingLabel);
        pendingLabel = NULL;
    }

    if(inProgressLabel != NULL)
    {
        for (int i = 0; i < size[1]; i++)
        {
            gtk_box_remove(GTK_BOX(box), GTK_WIDGET(inProgressLabel[i]));
        }

        free(inProgressLabel);
        inProgressLabel = NULL;
    }

    if(completedLabel != NULL)
    {
        for (int i = 0; i < size[2]; i++)
        {
            gtk_box_remove(GTK_BOX(box), GTK_WIDGET(completedLabel[i]));
        }

        free(completedLabel);
        completedLabel = NULL;
    }

    if(spaceLabel2 != NULL)
    {
        gtk_box_remove(GTK_BOX(box), GTK_WIDGET(*spaceLabel2));
        free(spaceLabel2);
        spaceLabel2 = NULL;
    }

    if(infoLabel != NULL)
    {
        for (int i = 0; i < size[3]; i++)
        {
            gtk_box_remove(GTK_BOX(box1), GTK_WIDGET(infoLabel[i]));
        }

        free(infoLabel);
        infoLabel = NULL;
    }

    if(bottomLabel != NULL)
    {
        gtk_box_remove(GTK_BOX(box1), GTK_WIDGET(*bottomLabel));
        free(bottomLabel);
        bottomLabel = NULL;
    }
    
    if(infoLabel != NULL)
    {
        for (int i = 0; i < size[3]; i++)
        {
            gtk_box_remove(GTK_BOX(box1), GTK_WIDGET(infoLabel[i]));
        }

        free(infoLabel);
        infoLabel = NULL;
    }

    if(bottomLabel != NULL)
    {
        gtk_box_remove(GTK_BOX(box1), GTK_WIDGET(*bottomLabel));
        free(bottomLabel);
        bottomLabel = NULL;
    }

    DestroyList(&L1);
    DestroyList(&L2);
    DestroyList(&L3);
    DestroyList(&L4);
    
    L1 = NULL;
    L2 = NULL;
    L3 = NULL;
    L4 = NULL;
}

static void on_activate(GtkApplication *app) 
{
    window = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (window), "Task Management System");
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

    label[0] = gtk_label_new ("");
    label[1] = gtk_label_new ("");
    label[2] = gtk_label_new ("");
    label[3] = gtk_label_new ("");

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, 
                                        "button { font-size: 20px; font-weight: normal; border-radius: 10px;} "
                                        "button:hover { font-size: 25px; font-weight: bold;} "
                                        ".bold-label { font-size: 25px; font-weight: bold;}"
                                        "entry { min-height: 50px; font-size: 20px; font-weight: 600; padding: 10px; border-radius: 10px;}"
                                        ".exist { color: red;}"
                                        ".correct { color: green;}"
                                        ".title-label { color: blue;font-size: 25px; font-weight: bold;}");
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

    retour[0] = gtk_button_new_with_mnemonic("Return");
    g_signal_connect (retour[0], "clicked", G_CALLBACK(Home), NULL);

    grid[1] = gtk_grid_new();
    
    buffer[0] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[0]), 12);

    entry[0] = gtk_entry_new_with_buffer(buffer[0]);

    buffer[1] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[1]), 1000);

    entry[1] = gtk_entry_new_with_buffer(buffer[1]);

    const char *items1[] = {"1", "2", "3", "4", "5", NULL};

    dropdown[0] = gtk_drop_down_new_from_strings(items1);

    const char *items2[] = {"Pending", "In Progress", "Completed", NULL};

    dropdown[1] = gtk_drop_down_new_from_strings(items2);
 
    submit[0] = gtk_button_new_with_mnemonic("Add");
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

    gtk_widget_add_css_class(GTK_WIDGET(label2[0]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[1]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[2]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label2[3]), "bold-label");

    gtk_label_set_label(GTK_LABEL(label2[1]), "Identifier (maximum 12 caracters):");
    gtk_label_set_xalign(GTK_LABEL(label2[1]), 0.0);

    buffer[2] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[2]), 12);

    entry[2] = gtk_entry_new_with_buffer(buffer[2]);

    submit[1] = gtk_button_new_with_mnemonic("Delete");
    retour[1] = gtk_button_new_with_mnemonic("Return");

    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[0]), 0, 0, 12, 2);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[1]), 1, 2, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(entry[2]), 1, 3, 3, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[2]), 1, 4, 10, 2);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(submit[1]), 1, 6, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(retour[1]), 6, 6, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[2]), GTK_WIDGET(label2[3]), 0, 7, 12, 1);
    
    g_signal_connect (submit[1], "clicked", G_CALLBACK(Delete), NULL);
    g_signal_connect (retour[1], "clicked", G_CALLBACK(Home), NULL);

    grid[3] = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid[3]), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid[3]), 10);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid[3]), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid[3]), TRUE);
    g_signal_connect (button[2], "clicked", G_CALLBACK(GoToUpdate), NULL);

    buffer[3] = gtk_entry_buffer_new(NULL, -1);
    gtk_entry_buffer_set_max_length(GTK_ENTRY_BUFFER(buffer[3]), 11);

    entry[3] = gtk_entry_new_with_buffer(buffer[3]);

    submit[2] = gtk_button_new_with_mnemonic("Update Status");
    g_signal_connect (submit[2], "clicked", G_CALLBACK(Update), NULL);
    retour[2] = gtk_button_new_with_mnemonic("Return");
    g_signal_connect (retour[2], "clicked", G_CALLBACK(Home), NULL);

    const char *items3[] = {"Pending", "In Progress", "Completed", NULL};

    dropdown[2] = gtk_drop_down_new_from_strings(items3);

    label3[0] = gtk_label_new("");
    label3[1] = gtk_label_new("");
    label3[2] = gtk_label_new("");
    label3[3] = gtk_label_new("");
    label3[4] = gtk_label_new("");
    gtk_widget_add_css_class(GTK_WIDGET(label3[0]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label3[1]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label3[2]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label3[3]), "bold-label");
    gtk_widget_add_css_class(GTK_WIDGET(label3[4]), "bold-label");

    gtk_label_set_label(GTK_LABEL(label3[1]), "Identifier (maximum 12 caracters):");
    gtk_label_set_xalign(GTK_LABEL(label3[1]), 0.0);
    gtk_label_set_label(GTK_LABEL(label3[2]), "New Status :");
    gtk_label_set_xalign(GTK_LABEL(label3[2]), 0.0);

    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(label3[0]), 0, 0, 12, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(label3[1]), 1, 1, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(entry[3]), 1, 2, 3, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(label3[2]), 1, 3, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(dropdown[2]), 1, 4, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(label3[3]), 1, 5, 10, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(submit[2]), 1, 6, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(retour[2]), 6, 6, 5, 1);
    gtk_grid_attach(GTK_GRID(grid[3]), GTK_WIDGET(label3[4]), 0, 7, 12, 1);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    g_signal_connect(button[3], "clicked", G_CALLBACK(GoToDisplay), NULL);

    retour[3] = gtk_button_new_with_mnemonic("Return");
    g_signal_connect (retour[3], "clicked", G_CALLBACK(Home), NULL);

    gtk_box_append(GTK_BOX(box), GTK_WIDGET(retour[3]));

    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[0]), "", 0);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[1]), "", 0);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[2]), "", 0);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer[3]), "", 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[0]), 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[1]), 0);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown[2]), 0);
    gtk_label_set_label(GTK_LABEL(label1[5]), "");
    gtk_label_set_label(GTK_LABEL(label2[2]), "");
    gtk_label_set_label(GTK_LABEL(label3[3]), "");

    spaceLabel1 = gtk_label_new_with_mnemonic("\n");

    gtk_box_append(GTK_BOX(box), GTK_WIDGET(spaceLabel1));

    scrolled_window1 = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1), 
                                   GTK_POLICY_NEVER,
                                   GTK_POLICY_AUTOMATIC
    );

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window1), box);

    box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(box1, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box1, GTK_ALIGN_CENTER);

    scrolled_window2 = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2), 
                                   GTK_POLICY_NEVER,
                                   GTK_POLICY_AUTOMATIC
    );

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window2), box1);

    g_signal_connect(button[4], "clicked", G_CALLBACK(GoToSearch), NULL);
    retour[4] = gtk_button_new_with_mnemonic("Return");
    g_signal_connect (retour[4], "clicked", G_CALLBACK(Home), NULL);
    gtk_box_append(GTK_BOX(box1), GTK_WIDGET(retour[4]));

    headLabel = gtk_label_new_with_mnemonic("\n");
    titleLabel = gtk_label_new_with_mnemonic("Priority Level :");
    gtk_widget_add_css_class(GTK_WIDGET(titleLabel), "bold-label");

    const char *items4[] = {"1", "2", "3", "4", "5", NULL};
    dropdown[3] = gtk_drop_down_new_from_strings(items1);

    submit[3] = gtk_button_new_with_label("Search");
    g_signal_connect(submit[3], "clicked", G_CALLBACK(Search), NULL);

    gtk_box_append(GTK_BOX(box1), GTK_WIDGET(headLabel));
    gtk_box_append(GTK_BOX(box1), GTK_WIDGET(titleLabel));
    gtk_box_append(GTK_BOX(box1), GTK_WIDGET(dropdown[3]));    
    gtk_box_append(GTK_BOX(box1), GTK_WIDGET(submit[3]));    

    gtk_stack_add_titled(GTK_STACK(stack), grid[0], "grid_home", "Home");
    gtk_stack_add_titled(GTK_STACK(stack), grid[1], "grid_insert", "Insert");
    gtk_stack_add_titled(GTK_STACK(stack), grid[2], "grid_delete", "Delete");
    gtk_stack_add_titled(GTK_STACK(stack), grid[3], "grid_update", "Update");
    gtk_stack_add_titled(GTK_STACK(stack), scrolled_window1, "grid_display", "Display");
    gtk_stack_add_titled(GTK_STACK(stack), scrolled_window2, "grid_search", "Search");

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[])
{
    GtkApplication *app;
    int status;
    L = NULL;
    L1 = NULL;
    L2 = NULL;
    L3 = NULL;
    L4 = NULL;
    pendingLabel = NULL;
    inProgressLabel = NULL;
    completedLabel = NULL;
    label4 = NULL;
    spaceLabel2 = NULL;

    file = fopen("tasks.dat", "rb");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    FileTask fileTask;
    Task *T;
    
    while (fread(&fileTask, sizeof(FileTask), 1, file) == 1)
    {
        T = malloc(sizeof(Task));
        strcpy(T->Id, fileTask.Id);
        strcpy(T->Description, fileTask.Description);
        strcpy(T->Status, fileTask.Status);
        T->PriorityLevel = fileTask.PriorityLevel;
        T->next = L;
        L = T;
    }

    fclose(file);
    
    app = gtk_application_new ("stackof.holger.entry", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_app_shutdown), NULL);

    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
