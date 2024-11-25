typedef struct Task Task;
struct Task
{
    char Id[12];
    char Description[1000];
    int PriorityLevel;
    char Status[15];
    Task* next;
};

int InsertTask(Task **L, char id[12], char description[1000], int priorityLevel, char status[15])
{
    if(L != NULL)
    {
        Task *P = malloc(sizeof(Task));
        strcpy(P->Id, id);
        P->PriorityLevel = priorityLevel;
        strcpy(P->Description, description);
        strcpy(P->Status, status);

        if (*L == NULL)
        {
            P->next = NULL;
            *L = P;
            return 0;
        }

        Task *Q = *L;

        while(Q != NULL)
        {
            if(P->Id == Q->Id)
            {
                return 1;
            }
        }

        Q = *L;
        
        if((*L)->PriorityLevel < priorityLevel)
        {
            P->next = *L;
            *L = P;
            return 0;
        }

        while (Q->next != NULL && Q->next->PriorityLevel > priorityLevel)
        {
            Q = Q->next;
        }
        
        P->next = Q->next;
        Q->next = P;

        return 0;
        
    }
}
