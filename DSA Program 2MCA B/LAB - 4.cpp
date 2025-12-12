#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

// ------------------- Structures -------------------
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    int numVertices;
    char labels[MAX][50];
    Node* adjList[MAX];
    int indegree[MAX];
} Graph;

// ------------------- Function Prototypes -------------------
void initGraph(Graph* g);
int findIndex(Graph* g, char* label);
void addVertex(Graph* g, char* label);
void addEdge(Graph* g, char* from, char* to);
void removeEdge(Graph* g, char* from, char* to);
void removeVertex(Graph* g, char* label);
void clearGraph(Graph* g);
void displayGraph(Graph* g);
int detectCycle(Graph* g);
void topologicalSort(Graph* g);
void loadSampleDataset(Graph* g);
void buildMenu(Graph* g);

// ------------------- Utility -------------------
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// ------------------- Initialize Graph -------------------
void initGraph(Graph* g) {
    g->numVertices = 0;
    for (int i = 0; i < MAX; i++) {
        g->adjList[i] = NULL;
        g->indegree[i] = 0;
    }
}

// ------------------- Find Vertex -------------------
int findIndex(Graph* g, char* label) {
    for (int i = 0; i < g->numVertices; i++) {
        if (strcmp(g->labels[i], label) == 0)
            return i;
    }
    return -1;
}

// ------------------- Add Vertex -------------------
void addVertex(Graph* g, char* label) {
    if (findIndex(g, label) != -1) {
        printf("Task '%s' already exists!\n", label);
        return;
    }
    strcpy(g->labels[g->numVertices++], label);
    printf("? Task '%s' added successfully.\n", label);
}

// ------------------- Add Edge -------------------
void addEdge(Graph* g, char* from, char* to) {
    int u = findIndex(g, from);
    int v = findIndex(g, to);
    if (u == -1 || v == -1) {
        printf("? Invalid task names.\n");
        return;
    }
    if (u == v) {
        printf("? Self-loop not allowed.\n");
        return;
    }

    Node* temp = g->adjList[u];
    while (temp) {
        if (temp->vertex == v) {
            printf("? Parallel edge not allowed.\n");
            return;
        }
        temp = temp->next;
    }

    Node* newNode = createNode(v);
    newNode->next = g->adjList[u];
    g->adjList[u] = newNode;
    g->indegree[v]++;

    // Check cycle
    if (detectCycle(g)) {
        g->adjList[u] = g->adjList[u]->next;
        g->indegree[v]--;
        printf("? Edge creates a cycle! Not added.\n");
    } else {
        printf("? Dependency added: %s ? %s\n", from, to);
    }
}

// ------------------- Remove Edge -------------------
void removeEdge(Graph* g, char* from, char* to) {
    int u = findIndex(g, from);
    int v = findIndex(g, to);
    if (u == -1 || v == -1) return;

    Node* temp = g->adjList[u];
    Node* prev = NULL;
    while (temp) {
        if (temp->vertex == v) {
            if (prev) prev->next = temp->next;
            else g->adjList[u] = temp->next;
            g->indegree[v]--;
            free(temp);
            printf("? Edge %s ? %s removed.\n", from, to);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("? Edge not found.\n");
}

// ------------------- Remove Vertex -------------------
void removeVertex(Graph* g, char* label) {
    int idx = findIndex(g, label);
    if (idx == -1) {
        printf("? Task not found.\n");
        return;
    }
    // Remove all edges to this vertex
    for (int i = 0; i < g->numVertices; i++) {
        if (g->adjList[i]) {
            removeEdge(g, g->labels[i], label);
        }
    }
    // Remove all edges from this vertex
    Node* temp = g->adjList[idx];
    while (temp) {
        g->indegree[temp->vertex]--;
        Node* del = temp;
        temp = temp->next;
        free(del);
    }
    g->adjList[idx] = NULL;

    // Shift vertices
    for (int i = idx; i < g->numVertices - 1; i++) {
        strcpy(g->labels[i], g->labels[i + 1]);
        g->adjList[i] = g->adjList[i + 1];
        g->indegree[i] = g->indegree[i + 1];
    }
    g->numVertices--;
    printf("? Task '%s' removed.\n", label);
}

// ------------------- Clear Graph -------------------
void clearGraph(Graph* g) {
    for (int i = 0; i < g->numVertices; i++) {
        Node* temp = g->adjList[i];
        while (temp) {
            Node* del = temp;
            temp = temp->next;
            free(del);
        }
        g->adjList[i] = NULL;
        g->indegree[i] = 0;
    }
    g->numVertices = 0;
    printf("?? Graph cleared.\n");
}

// ------------------- Display Graph -------------------
void displayGraph(Graph* g) {
    printf("\n--- Cloud Task Graph (Adjacency List) ---\n");
    for (int i = 0; i < g->numVertices; i++) {
        printf("%s ? ", g->labels[i]);
        Node* temp = g->adjList[i];
        while (temp) {
            printf("%s ", g->labels[temp->vertex]);
            temp = temp->next;
        }
        printf("\n");
    }
}

// ------------------- Cycle Detection (DFS) -------------------
int cycleUtil(Graph* g, int v, int visited[], int recStack[]) {
    if (!visited[v]) {
        visited[v] = 1;
        recStack[v] = 1;
        Node* temp = g->adjList[v];
        while (temp) {
            if (!visited[temp->vertex] && cycleUtil(g, temp->vertex, visited, recStack))
                return 1;
            else if (recStack[temp->vertex])
                return 1;
            temp = temp->next;
        }
    }
    recStack[v] = 0;
    return 0;
}

int detectCycle(Graph* g) {
    int visited[MAX] = {0}, recStack[MAX] = {0};
    for (int i = 0; i < g->numVertices; i++)
        if (cycleUtil(g, i, visited, recStack))
            return 1;
    return 0;
}

// ------------------- Topological Sort (Kahn’s Algorithm) -------------------
void topologicalSort(Graph* g) {
    int indeg[MAX];
    memcpy(indeg, g->indegree, sizeof(g->indegree));
    int queue[MAX], front = 0, rear = 0;
    char order[MAX][50];
    int count = 0;

    for (int i = 0; i < g->numVertices; i++)
        if (indeg[i] == 0)
            queue[rear++] = i;

    while (front < rear) {
        int v = queue[front++];
        strcpy(order[count++], g->labels[v]);
        Node* temp = g->adjList[v];
        while (temp) {
            indeg[temp->vertex]--;
            if (indeg[temp->vertex] == 0)
                queue[rear++] = temp->vertex;
            temp = temp->next;
        }
    }

    if (count != g->numVertices) {
        printf("\n? Cycle detected — no valid scheduling order.\n");
        return;
    }

    printf("\n? Valid Cloud Task Execution Order:\n");
    for (int i = 0; i < count; i++)
        printf("%s ", order[i]);
    printf("\nTime Complexity: O(V + E)\n");
}

// ------------------- Sample Dataset -------------------
void loadSampleDataset(Graph* g) {
    clearGraph(g);
    printf("\nLoaded Sample Dataset: Cloud Task Scheduling\n");
    addVertex(g, "Task_Login");
    addVertex(g, "Task_DataLoad");
    addVertex(g, "Task_SecurityCheck");
    addVertex(g, "Task_Compute");
    addVertex(g, "Task_Report");

    addEdge(g, "Task_Login", "Task_DataLoad");
    addEdge(g, "Task_DataLoad", "Task_SecurityCheck");
    addEdge(g, "Task_SecurityCheck", "Task_Compute");
    addEdge(g, "Task_Compute", "Task_Report");
}

// ------------------- Build Menu -------------------
void buildMenu(Graph* g) {
    int c;
    char a[50], b[50];
    do {
        printf("\n--- Build/Reset Graph Menu ---\n");
        printf("1. Add Task (Vertex)\n2. Add Dependency (Edge)\n3. Remove Edge\n4. Remove Task\n5. Clear Graph\n6. Back\nChoice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: printf("Enter task name: "); scanf("%s", a); addVertex(g, a); break;
            case 2: printf("Enter edge (from to): "); scanf("%s %s", a, b); addEdge(g, a, b); break;
            case 3: printf("Enter edge (from to): "); scanf("%s %s", a, b); removeEdge(g, a, b); break;
            case 4: printf("Enter task to remove: "); scanf("%s", a); removeVertex(g, a); break;
            case 5: clearGraph(g); break;
            case 6: return;
        }
    } while (1);
}

// ------------------- Main Menu -------------------
int main() {
    Graph g;
    initGraph(&g);
    int choice;

    do {
        printf("\n=== Cloud Task Scheduling System ===\n");
        printf("1. Build/Reset Graph (DAG)\n");
        printf("2. Topological Sort\n");
        printf("3. Detect Cycle\n");
        printf("4. Load Sample Dataset\n");
        printf("5. Display Graph\n");
        printf("6. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: buildMenu(&g); break;
            case 2: topologicalSort(&g); break;
            case 3:
                if (detectCycle(&g))
                    printf("? Cycle Found! Graph is not a DAG.\n");
                else
                    printf("? No Cycle. Graph is a valid DAG.\n");
                break;
            case 4: loadSampleDataset(&g); break;
            case 5: displayGraph(&g); break;
            case 6: exit(0);
            default: printf("Invalid choice.\n");
        }
    } while (1);
    return 0;
}

