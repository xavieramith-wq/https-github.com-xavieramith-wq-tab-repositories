#include <stdio.h>
#include <stdlib.h>

#define MAX 50
#define INF 9999

// -------------------- FRACTIONAL KNAPSACK --------------------
void fractionalKnapsack() {
    int n, i;
    float capacity, totalProfit = 0;

    printf("\nEnter number of tasks: ");
    scanf("%d", &n);

    float weight[n], profit[n], ratio[n];

    printf("Enter profit and weight for each task:\n");
    for (i = 0; i < n; i++) {
        scanf("%f %f", &profit[i], &weight[i]);
        ratio[i] = profit[i] / weight[i];
    }

    printf("Enter total cloud capacity: ");
    scanf("%f", &capacity);

    // Sort by ratio in descending order
    for (i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (ratio[i] < ratio[j]) {
                float t = ratio[i]; ratio[i] = ratio[j]; ratio[j] = t;
                t = profit[i]; profit[i] = profit[j]; profit[j] = t;
                t = weight[i]; weight[i] = weight[j]; weight[j] = t;
            }
        }
    }

    float x[n];
    for (i = 0; i < n; i++) x[i] = 0;

    float remaining = capacity;

    for (i = 0; i < n; i++) {
        if (weight[i] <= remaining) {
            x[i] = 1;
            remaining -= weight[i];
            totalProfit += profit[i];
        } else {
            x[i] = remaining / weight[i];
            totalProfit += profit[i] * x[i];
            break;
        }
    }

    printf("\nFractional Knapsack Task Selection:\n");
    for (i = 0; i < n; i++)
        printf("Task %d -> %.2f%%\n", i + 1, x[i] * 100);

    printf("Total Profit: %.2f\n", totalProfit);
}

// -------------------- 0/1 KNAPSACK --------------------
void zeroOneKnapsack() {
    int n, i, W;

    printf("\nEnter number of tasks: ");
    scanf("%d", &n);

    int weight[n], profit[n];

    printf("Enter profit and weight of each task:\n");
    for (i = 0; i < n; i++)
        scanf("%d %d", &profit[i], &weight[i]);

    printf("Enter total cloud capacity: ");
    scanf("%d", &W);

    int dp[n + 1][W + 1];

    for (i = 0; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (weight[i - 1] <= j)
                dp[i][j] = (profit[i - 1] + dp[i - 1][j - weight[i - 1]] >
                            dp[i - 1][j])
                               ? profit[i - 1] + dp[i - 1][j - weight[i - 1]]
                               : dp[i - 1][j];
            else
                dp[i][j] = dp[i - 1][j];
        }
    }

    printf("Maximum Profit (0/1 Knapsack): %d\n", dp[n][W]);
}

// -------------------- PRIM'S MST --------------------
void primMST() {
    int n;

    printf("\nEnter number of servers: ");
    scanf("%d", &n);

    int cost[MAX][MAX], selected[MAX];

    printf("Enter cost adjacency matrix (0 for no edge):\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            if (cost[i][j] == 0)
                cost[i][j] = INF;
        }
    }

    for (int i = 0; i < n; i++)
        selected[i] = 0;

    selected[0] = 1;

    int edges = 0, total = 0;

    printf("Edges in MST (Prim's):\n");

    while (edges < n - 1) {
        int min = INF, x = 0, y = 0;

        for (int i = 0; i < n; i++) {
            if (selected[i]) {
                for (int j = 0; j < n; j++) {
                    if (!selected[j] && cost[i][j] < min) {
                        min = cost[i][j];
                        x = i;
                        y = j;
                    }
                }
            }
        }

        printf("%d - %d : %d\n", x, y, cost[x][y]);
        total += cost[x][y];
        selected[y] = 1;
        edges++;
    }

    printf("Total cost (Prim's): %d\n", total);
}

// -------------------- KRUSKAL'S MST --------------------
int find(int parent[], int i) {
    while (parent[i] != i)
        i = parent[i];
    return i;
}

void unionSet(int parent[], int a, int b) {
    a = find(parent, a);
    b = find(parent, b);
    parent[a] = b;
}

void kruskalMST() {
    int n, edges;

    printf("\nEnter number of servers and edges: ");
    scanf("%d %d", &n, &edges);

    int u[edges], v[edges], w[edges];

    printf("Enter edges (u v cost):\n");
    for (int i = 0; i < edges; i++)
        scanf("%d %d %d", &u[i], &v[i], &w[i]);

    // Sort edges by weight
    for (int i = 0; i < edges - 1; i++) {
        for (int j = i + 1; j < edges; j++) {
            if (w[i] > w[j]) {
                int t = w[i]; w[i] = w[j]; w[j] = t;
                t = u[i]; u[i] = u[j]; u[j] = t;
                t = v[i]; v[i] = v[j]; v[j] = t;
            }
        }
    }

    int parent[n];
    for (int i = 0; i < n; i++)
        parent[i] = i;

    int total = 0, count = 0;

    printf("Edges in MST (Kruskal's):\n");

    for (int i = 0; i < edges && count < n - 1; i++) {
        int a = find(parent, u[i]);
        int b = find(parent, v[i]);

        if (a != b) {
            printf("%d - %d : %d\n", u[i], v[i], w[i]);
            total += w[i];
            unionSet(parent, a, b);
            count++;
        }
    }

    printf("Total cost (Kruskal's): %d\n", total);
}

// -------------------- MENU --------------------
int main() {
    int ch;

    do {
        printf("\n--- Cloud Task Scheduling Menu ---\n");
        printf("1. Fractional Knapsack\n");
        printf("2. 0/1 Knapsack\n");
        printf("3. Prim's MST\n");
        printf("4. Kruskal's MST\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: fractionalKnapsack(); break;   
            case 2: zeroOneKnapsack(); break;
            case 3: primMST(); break;
            case 4: kruskalMST(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (ch != 5);

    return 0;
}

