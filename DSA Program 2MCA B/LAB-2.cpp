#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10
#define MAX_TASK_NAME 50

// ---------- Task Structure ----------
typedef struct {
    int taskId;
    char taskName[MAX_TASK_NAME];
    int priority;
    int cpuTime;
} Task;

// ---------- Normal Queue ----------
typedef struct {
    Task tasks[MAX];
    int front, rear;
} NormalQueue;

// ---------- Circular Queue ----------
typedef struct {
    Task tasks[MAX];
    int front, rear, count;
} CircularQueue;

// ---------- Priority Queue (Min-Heap) ----------
typedef struct {
    Task tasks[MAX];
    int size;
} PriorityQueue;

// ---------- Deque ----------
typedef struct {
    Task tasks[MAX];
    int front, rear, count;
} Deque;

// ---------- Utility ----------
Task createTask(int id, const char* name, int pr, int cpuTime) {
    Task t;
    t.taskId = id;
    strcpy(t.taskName, name);
    t.priority = pr;
    t.cpuTime = cpuTime;
    return t;
}

void printTableHeader(const char* queueType) {
    printf("\n=== %s ===\n", queueType);
    printf("%-8s | %-20s | %-8s | %-8s\n", "TaskID", "Task Name", "Priority", "CPU(s)");
    printf("----------------------------------------------------\n");
}

void printTaskRow(Task t) {
    if(t.taskId != -1)
        printf("%-8d | %-20s | %-8d | %-8d\n", t.taskId, t.taskName, t.priority, t.cpuTime);
}

// ---------- Normal Queue Functions ----------
void initNormalQueue(NormalQueue *q){ q->front=0; q->rear=-1; }
int isNormalEmpty(NormalQueue *q){ return q->rear < q->front; }
int isNormalFull(NormalQueue *q){ return q->rear == MAX-1; }

void enqueueNormal(NormalQueue *q, Task t){
    if(isNormalFull(q)){ printf("Overflow!\n"); return; }
    q->tasks[++q->rear] = t;
    printf("Enqueued Task %d - %s\n", t.taskId, t.taskName);
}

Task dequeueNormal(NormalQueue *q){
    Task empty = {-1,"",0,0};
    if(isNormalEmpty(q)){ printf("Underflow!\n"); return empty; }
    Task t = q->tasks[q->front++];
    printf("Dequeued Task %d - %s\n", t.taskId, t.taskName);
    return t;
}

void displayNormal(NormalQueue *q){
    if(isNormalEmpty(q)){ printf("Empty Queue!\n"); return; }
    printTableHeader("Normal Queue");
    for(int i=q->front;i<=q->rear;i++) printTaskRow(q->tasks[i]);
}

void initCircularQueue(CircularQueue *q){ q->front=0; q->rear=-1; q->count=0; }
int isCircularEmpty(CircularQueue *q){ return q->count==0; }
int isCircularFull(CircularQueue *q){ return q->count==MAX; }

void enqueueCircular(CircularQueue *q, Task t){
    if(isCircularFull(q)){ printf("Overflow!\n"); return; }
    q->rear=(q->rear+1)%MAX; q->tasks[q->rear]=t; q->count++;
    printf("Enqueued Task %d - %s\n", t.taskId, t.taskName);
}

Task dequeueCircular(CircularQueue *q){
    Task empty = {-1,"",0,0};
    if(isCircularEmpty(q)){ printf("Underflow!\n"); return empty; }
    Task t = q->tasks[q->front]; q->front=(q->front+1)%MAX; q->count--;
    printf("Dequeued Task %d - %s\n", t.taskId, t.taskName);
    return t;
}

void displayCircular(CircularQueue *q){
    if(isCircularEmpty(q)){ printf("Empty Queue!\n"); return; }
    printTableHeader("Circular Queue");
    int idx=q->front;
    for(int i=0;i<q->count;i++){ printTaskRow(q->tasks[idx]); idx=(idx+1)%MAX; }
}

void initPriorityQueue(PriorityQueue *q){ q->size=0; }

int isPriorityEmpty(PriorityQueue *q){ return q->size==0; }
int isPriorityFull(PriorityQueue *q){ return q->size==MAX; }

void swap(Task *a, Task *b){ Task t=*a; *a=*b; *b=t; }

void heapifyUp(PriorityQueue *q, int idx){
    while(idx>0){
        int parent=(idx-1)/2;
        if(q->tasks[idx].priority<q->tasks[parent].priority){ swap(&q->tasks[idx], &q->tasks[parent]); idx=parent; }
        else break;
    }
}

void heapifyDown(PriorityQueue *q, int idx){
    int left=2*idx+1, right=2*idx+2, smallest=idx;
    if(left<q->size && q->tasks[left].priority<q->tasks[smallest].priority) smallest=left;
    if(right<q->size && q->tasks[right].priority<q->tasks[smallest].priority) smallest=right;
    if(smallest!=idx){ swap(&q->tasks[smallest], &q->tasks[idx]); heapifyDown(q, smallest); }
}

void enqueuePriority(PriorityQueue *q, Task t){
    if(isPriorityFull(q)){ printf("Overflow!\n"); return; }
    q->tasks[q->size]=t; heapifyUp(q,q->size); q->size++;
    printf("Scheduled Task %d - %s with Priority %d\n", t.taskId, t.taskName, t.priority);
}

Task dequeuePriority(PriorityQueue *q){
    Task empty = {-1,"",0,0};
    if(isPriorityEmpty(q)){ printf("Underflow!\n"); return empty; }
    Task t = q->tasks[0];
    q->tasks[0]=q->tasks[q->size-1]; q->size--; if(q->size>0) heapifyDown(q,0);
    printf("Executing Task %d - %s with Priority %d\n", t.taskId, t.taskName, t.priority);
    return t;
}

void displayPriority(PriorityQueue *q){
    if(isPriorityEmpty(q)){ printf("Empty Priority Queue!\n"); return; }
    printTableHeader("Priority Queue (Min-Heap)");
    for(int i=0;i<q->size;i++) printTaskRow(q->tasks[i]);
}


void initDeque(Deque *q){ q->front=0; q->rear=-1; q->count=0; }
int isDequeEmpty(Deque *q){ return q->count==0; }
int isDequeFull(Deque *q){ return q->count==MAX; }

void insertFront(Deque *q, Task t){
    if(isDequeFull(q)){ printf("Overflow!\n"); return; }
    q->front=(q->front-1+MAX)%MAX; q->tasks[q->front]=t; q->count++;
    printf("Inserted Front Task %d - %s\n", t.taskId, t.taskName);
}

void insertRear(Deque *q, Task t){
    if(isDequeFull(q)){ printf("Overflow!\n"); return; }
    q->rear=(q->rear+1)%MAX; q->tasks[q->rear]=t; q->count++;
    printf("Inserted Rear Task %d - %s\n", t.taskId, t.taskName);
}

Task deleteFront(Deque *q){
    Task empty = {-1,"",0,0};
    if(isDequeEmpty(q)){ printf("Underflow!\n"); return empty; }
    Task t=q->tasks[q->front]; q->front=(q->front+1)%MAX; q->count--;
    printf("Deleted Front Task %d - %s\n", t.taskId, t.taskName);
    return t;
}

Task deleteRear(Deque *q){
    Task empty = {-1,"",0,0};
    if(isDequeEmpty(q)){ printf("Underflow!\n"); return empty; }
    Task t=q->tasks[q->rear]; q->rear=(q->rear-1+MAX)%MAX; q->count--;
    printf("Deleted Rear Task %d - %s\n", t.taskId, t.taskName);
    return t;
}

void displayDeque(Deque *q){
    if(isDequeEmpty(q)){ printf("Empty Deque!\n"); return; }
    printTableHeader("Deque");
    int idx=q->front;
    for(int i=0;i<q->count;i++){ printTaskRow(q->tasks[idx]); idx=(idx+1)%MAX; }
}

// ---------- Main Menu ----------
int main(){
    NormalQueue nQ; initNormalQueue(&nQ);
    CircularQueue cQ; initCircularQueue(&cQ);
    PriorityQueue pQ; initPriorityQueue(&pQ);
    Deque dQ; initDeque(&dQ);

    int choice, sub, id=1, pr, cpuTime;
    char name[MAX_TASK_NAME];

    while(1){
        printf("\n--- CLOUD TASK SCHEDULER ---\n");
        printf("1. Normal Queue  2. Circular Queue  3. Priority Queue  4. Deque  5. Exit\nChoice: ");
        scanf("%d",&choice);

        if(choice==5) break;

        switch(choice){
            case 1:
                printf("1.Enqueue 2.Dequeue 3.Display: "); scanf("%d",&sub);
                if(sub==1){
                    printf("Task Name: "); scanf("%s", name);
                    printf("CPU Time: "); scanf("%d", &cpuTime);
                    enqueueNormal(&nQ, createTask(id++, name, 0, cpuTime));
                }
                else if(sub==2) dequeueNormal(&nQ);
                else displayNormal(&nQ);
                break;

            case 2:
                printf("1.Enqueue 2.Dequeue 3.Display: "); scanf("%d",&sub);
                if(sub==1){
                    printf("Task Name: "); scanf("%s", name);
                    printf("CPU Time: "); scanf("%d", &cpuTime);
                    enqueueCircular(&cQ, createTask(id++, name, 0, cpuTime));
                }
                else if(sub==2) dequeueCircular(&cQ);
                else displayCircular(&cQ);
                break;

            case 3:
                printf("1.Enqueue 2.Dequeue 3.Display: "); scanf("%d",&sub);
                if(sub==1){
                    printf("Task Name: "); scanf("%s", name);
                    printf("Priority: "); scanf("%d",&pr);
                    printf("CPU Time: "); scanf("%d", &cpuTime);
                    enqueuePriority(&pQ, createTask(id++, name, pr, cpuTime));
                }
                else if(sub==2) dequeuePriority(&pQ);
                else displayPriority(&pQ);
                break;

            case 4:
                printf("1.InsertFront 2.InsertRear 3.DeleteFront 4.DeleteRear 5.Display: "); scanf("%d",&sub);
                if(sub==1){
                    printf("Task Name: "); scanf("%s", name);
                    printf("CPU Time: "); scanf("%d", &cpuTime);
                    insertFront(&dQ, createTask(id++, name, 0, cpuTime));
                }
                else if(sub==2){
                    printf("Task Name: "); scanf("%s", name);
                    printf("CPU Time: "); scanf("%d", &cpuTime);
                    insertRear(&dQ, createTask(id++, name, 0, cpuTime));
                }
                else if(sub==3) deleteFront(&dQ);
                else if(sub==4) deleteRear(&dQ);
                else displayDeque(&dQ);
                break;

            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}

