#ifndef TASK
#define TASK

typedef struct Task Task;
struct Task
{
    char Id[13];    
    char Description[1001];
    int PriorityLevel;
    char Status[15];
    Task* next;
};

typedef struct FileTask FileTask;
struct FileTask
{
    char Id[13];    
    char Description[1001];
    int PriorityLevel;
    char Status[15];
};


#endif