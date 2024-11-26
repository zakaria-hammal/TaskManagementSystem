#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
    int ID;
    char description[200];
    int priority;
    char status[20];
    struct node* next;
} node;

void Taskdelete(node** head, int identifier) {
    node* current = *head;
    node* prev = NULL;

    if (current != NULL && current->ID == identifier) {
        *head = current->next;
        free(current);
        return;
    }

    while (current != NULL && current->ID != identifier) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        prev->next = current->next;
        free(current);
    } else {
        printf("Task does not exist\n");
    }
}

void Updatestatus(node** head, int identifier) {
    char newstat[20];
    printf("Enter new status: ");
    scanf("%s", newstat);

    node* current = *head;

    while (current != NULL && current->ID != identifier) {
        current = current->next;
    }

    if (current != NULL) {
        strcpy(current->status, newstat);
    } else {
        printf("Task does not exist\n");
    }
}

void displayByTask(node* head, char status[20]) {
    node* p = head;
    while (p != NULL) {
        if (strcmp(status, p->status) == 0) {
            printf("ID: %d, Priority: %d, Description: %s, Status: %s\n",
                   p->ID, p->priority, p->description, p->status);
        }
        p = p->next;
    }
}

void DisplayTask(node* head) {
    printf("Printing tasks with 'Pending' status:\n");
    displayByTask(head, "Pending");

    printf("Printing tasks with 'In Progress' status:\n");
    displayByTask(head, "In Progress");

    printf("Printing tasks with 'Completed' status:\n");
    displayByTask(head, "Completed");
}

void SearchByPriority(node* head, int priority) {
    node* p = head;
    bool found = false;

    while (p != NULL) {
        if (p->priority == priority) {
            printf("Priority: %d, ID: %d, Description: %s, Status: %s\n",
                   p->priority, p->ID, p->description, p->status);
            found = true;
        }
        p = p->next;
    }

    if (!found) {
        printf("No tasks with this priority were found\n");
    }
}

int main() {
    node* head = (node*)malloc(sizeof(node));
    if (head == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    head->ID = 1;
    strcpy(head->description, "Example Task");
    head->priority = 1;
    strcpy(head->status, "Pending");
    head->next = NULL;

    DisplayTask(head);

    free(head);
    return 0;
}
