typedef struct Task Task;
struct Task
{
    char Id[13];
    char Description[1001];
    int PriorityLevel;
    char Status[15];
    Task* next;
};

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