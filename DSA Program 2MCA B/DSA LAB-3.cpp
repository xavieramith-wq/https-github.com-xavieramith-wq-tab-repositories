#include <stdio.h>
#define MAX 30


typedef struct {
    int id;
    int priority;
    int execTime;
    char status[20];
} Task;

Task heap[MAX];
int size = 0;
int nextID = 100; 


void swap(Task *a, Task *b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}


void insertTask(int id, int priority, int execTime) {
    if (size >= MAX - 1) {
        printf("Heap Full! Cannot insert more tasks.\n");
        return;
    }

    if (id == 0) id = nextID++; 
    Task newTask = {id, priority, execTime, "Pending"};
    heap[++size] = newTask;

    int i = size;
    while (i > 1 && heap[i].priority < heap[i / 2].priority) {
        swap(&heap[i], &heap[i / 2]);
        i /= 2;
    }
    printf("Task ID %d inserted successfully!\n", id);
}


Task removeTask() {
    Task top = heap[1];
    heap[1] = heap[size--];

    int i = 1;
    while (2 * i <= size) {
        int smallest = i;
        int left = 2 * i, right = 2 * i + 1;

        if (heap[left].priority < heap[smallest].priority)
            smallest = left;
        if (right <= size && heap[right].priority < heap[smallest].priority)
            smallest = right;

        if (smallest == i) break;
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return top;
}


void displayTasks() {
    if (size == 0) {
        printf("No tasks scheduled.\n");
        return;
    }

    printf("\n=== Current Cloud Task Queue (Min-Heap) ===\n");
    int totalTime = 0;
    for (int i = 1; i <= size; i++) {
        printf("TaskID=%d | Priority=%d | Time=%d | Status=%s\n",
               heap[i].id, heap[i].priority, heap[i].execTime, heap[i].status);
        totalTime += heap[i].execTime;
    }
    printf("---------------------------------------------\n");
    printf("Total Tasks: %d | Average Execution Time: %.2f units\n",
           size, (float)totalTime / size);
}


void ageTasks() {
    for (int i = 1; i <= size; i++) {
        if (heap[i].priority > 1)
            heap[i].priority -= 1; 
    }
}

// Main function
int main() {
    int choice, id, priority, time;

    printf("=== Cloud Task Scheduler using Priority Queue (Enhanced) ===\n");

    while (1) {
        printf("\n1. Insert Task\n2. Execute Next Task\n3. Display Tasks\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter Task ID (0 for auto), Priority (1=High), Exec Time: ");
            scanf("%d%d%d", &id, &priority, &time);
            insertTask(id, priority, time);
            break;

        case 2:
            if (size == 0)
                printf("No tasks to execute!\n");
            else {
                Task t = removeTask();
                printf("\nExecuting Task ID=%d (Priority=%d, Time=%d)... Done!\n",
                       t.id, t.priority, t.execTime);
                printf("Task %d completed successfully.\n", t.id);
                ageTasks(); 
            }
            break;

        case 3:
            displayTasks();
            break;

        case 4:
            printf("Exiting Cloud Scheduler... Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice! Try again.\n");
        }
    }
}

