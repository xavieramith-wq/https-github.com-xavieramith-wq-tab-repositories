#include <stdio.h>
#define INF 99999
#define MAX 50

// Function to validate input
int readPositiveInt() {
    int x;
    while (1) {
        if (scanf("%d", &x) == 1 && x >= 0) return x;
        printf("Invalid input! Enter a positive integer: ");
        while (getchar() != '\n'); // clear buffer
    }
}

// Print matrix neatly
void printMatrix(int n, int dist[MAX][MAX]) {
    printf("\nFinal Minimum Latency Matrix (ms):\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(dist[i][j] == INF) printf(" INF ");
            else printf(" %3d ", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n;

    printf("Enter number of cloud servers (1-50): ");
    while(1) {
        if(scanf("%d", &n) == 1 && n > 0 && n <= MAX) break;
        printf("Invalid! Enter a number between 1 and 50: ");
        while(getchar() != '\n'); // flush
    }

    int dist[MAX][MAX];

    printf("\nEnter latency matrix (ms).\nUse %d for no direct connection.\n", INF);

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            printf("Latency from Server %d ? %d: ", i, j);

            int val = readPositiveInt();

            if(val == INF || i == j)
                dist[i][j] = (i == j) ? 0 : INF; // self loop = 0
            else
                dist[i][j] = val;
        }
    }

    // Floyd–Warshall
    for(int k=0; k<n; k++) {
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                if(dist[i][k] != INF && dist[k][j] != INF &&
                   dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printMatrix(n, dist);

    return 0;
}

