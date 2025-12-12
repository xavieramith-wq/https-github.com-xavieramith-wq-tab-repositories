#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
#define INF 99999

int readPositiveInt() {
    int x;
    while (1) {
        if (scanf("%d", &x) == 1 && x >= 0) return x;
        while (getchar() != '\n');
    }
}

// -------------------- FRACTIONAL KNAPSACK --------------------
void fractionalKnapsack() {
    int n, i;
    float capacity, totalProfit = 0;

    scanf("%d", &n);

    float weight[n], profit[n], ratio[n];

    for (i = 0; i < n; i++) {
        scanf("%f %f", &profit[i], &weight[i]);
        ratio[i] = profit[i] / weight[i];
    }

    scanf("%f", &capacity);

    for (i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (ratio[i] < ratio[j]) {
                float t = ratio[i]; ratio[i] = ratio[j]; ratio[j] = t;
                t = profit[i]; profit[i] = profit[j]; profit[j] = t;
                t = weight[i]; weight[i] = weight[j]; weight[j] = t;
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

    for (i = 0; i < n; i++)
        printf("Task %d -> %.2f%%\n", i + 1, x[i] * 100);

    printf("Total Profit: %.2f\n", totalProfit);
}

// -------------------- 0/1 KNAPSACK --------------------
void zeroOneKnapsack() {
    int n, i, W;
    scanf("%d", &n);

    int weight[n], profit[n];

    for (i = 0; i < n; i++)
        scanf("%d %d", &profit[i], &weight[i]);

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
    scanf("%d", &n);

    int cost[MAX][MAX], selected[MAX];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            if (cost[i][j] == 0)
                cost[i][j] = INF;
        }

    for (int i = 0; i < n; i++)
        selected[i] = 0;

    selected[0] = 1;

    int edges = 0, total = 0;

    while (edges < n - 1) {
        int min = INF, x = 0, y = 0;

        for (int i = 0; i < n; i++)
            if (selected[i])
                for (int j = 0; j < n; j++)
                    if (!selected[j] && cost[i][j] < min) {
                        min = cost[i][j];
                        x = i;
                        y = j;
                    }

        printf("%d - %d : %d\n", x, y, cost[x][y]);
        total += cost[x][y];
        selected[y] = 1;
        edges++;
    }

    printf("Total cost (Prim's): %d\n", total);
}

// -------------------- KRUSKAL FUNCTIONS --------------------
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

// -------------------- KRUSKAL MST --------------------
void kruskalMST() {
    int n, edges;
    scanf("%d %d", &n, &edges);

    int u[edges], v[edges], w[edges];

    for (int i = 0; i < edges; i++)
        scanf("%d %d %d", &u[i], &v[i], &w[i]);

    for (int i = 0; i < edges - 1; i++)
        for (int j = i + 1; j < edges; j++)
            if (w[i] > w[j]) {
                int t = w[i]; w[i] = w[j]; w[j] = t;
                t = u[i]; u[i] = u[j]; u[j] = t;
                t = v[i]; v[i] = v[j]; v[j] = t;
            }

    int parent[n];
    for (int i = 0; i < n; i++)
        parent[i] = i;

    int total = 0, count = 0;

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

// -------------------- LCS VALIDATION --------------------
int isValid(char str[]) {
    if (strlen(str) == 0) return 0;

    for (int i = 0; i < strlen(str); i++)
        if (!(isalnum(str[i]) || str[i] == '_'))
            return 0;

    return 1;
}

// -------------------- COMPUTE LCS --------------------
void computeLCS(char X[], char Y[]) {
    int m = strlen(X);
    int n = strlen(Y);

    int L[m + 1][n + 1];

    for (int i = 0; i <= m; i++)
        for (int j = 0; j <= n; j++)
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = 1 + L[i - 1][j - 1];
            else
                L[i][j] = (L[i - 1][j] > L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1];

    printf("LCS Length = %d\n", L[m][n]);

    int idx = L[m][n];
    char lcs[idx + 1];
    lcs[idx] = '\0';

    int i = m, j = n;

    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[idx - 1] = X[i - 1];
            i--; j--; idx--;
        }
        else if (L[i - 1][j] > L[i][j - 1])
            i--;
        else
            j--;
    }

    printf("LCS = %s\n", lcs);
}

// -------------------- FLOYD WARSHALL --------------------
void floydWarshall() {
    int n;
    scanf("%d", &n);

    int dist[MAX][MAX];

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++) {
            int val;
            scanf("%d", &val);
            if(val == INF || i == j)
                dist[i][j] = (i == j) ? 0 : INF;
            else
                dist[i][j] = val;
        }

    for(int k=0;k<n;k++)
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(dist[i][j] == INF) printf("INF ");
            else printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
}

// -------------------- MENU --------------------
int main() {
    int ch;

    do {
        printf("\n1. Fractional Knapsack\n");
        printf("2. 0/1 Knapsack\n");
        printf("3. Prim's MST\n");
        printf("4. Kruskal's MST\n");
        printf("5. LCS\n");
        printf("6. Floyd Warshall\n");
        printf("7. Exit\n");

        scanf("%d", &ch);

        if (ch == 1) fractionalKnapsack();
        else if (ch == 2) zeroOneKnapsack();
        else if (ch == 3) primMST();
        else if (ch == 4) kruskalMST();
        else if (ch == 5) {
            char X[100], Y[100];
            scanf("%s", X);
            scanf("%s", Y);
            computeLCS(X, Y);
        }
        else if (ch == 6) floydWarshall();
        else if (ch != 7) printf("Invalid Choice\n");

    } while (ch != 7);

    return 0;
}

