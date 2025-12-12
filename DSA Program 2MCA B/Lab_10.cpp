#include <stdio.h>

#define MAX 100

// Cloud Storage - Minimum Vertex Cover using simple 2-approximation
// Data stored in simple arrays (edge list)

int main() {
    int n, m;
    int U[MAX], V[MAX];
    int used[MAX] = {0};
    int cover[MAX] = {0};

    printf("Enter number of cloud storage nodes: ");
    scanf("%d", &n);

    printf("Enter number of data-transfer links: ");
    scanf("%d", &m);

    printf("Enter the links (u v format):\n");
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &U[i], &V[i]);
    }

    // ----- Minimum Vertex Cover Logic -----
    for (int i = 0; i < m; i++) {
        int u = U[i];
        int v = V[i];

        // If both nodes are free, add both to the cover
        if (!used[u] && !used[v]) {
            cover[u] = 1;
            cover[v] = 1;
            used[u] = used[v] = 1;
        }
    }

    // ----- Output -----
    printf("\nMinimum Vertex Cover (Critical Cloud Storage Nodes): ");
    for (int i = 0; i < n; i++) {
        if (cover[i])
            printf("%d ", i);
    }
    printf("\n");

    return 0;
}

