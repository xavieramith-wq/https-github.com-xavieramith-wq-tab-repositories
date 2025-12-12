#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to validate string (non-empty and contains valid characters)
int isValid(char str[]) {
    if (strlen(str) == 0) return 0;

    // Allow alphabets, numbers, underscores (task IDs)
    for (int i = 0; i < strlen(str); i++) {
        if (!(isalnum(str[i]) || str[i] == '_'))
            return 0;
    }
    return 1;
}

// Function to compute and print LCS using Bottom-Up DP
void computeLCS(char X[], char Y[]) {
    int m = strlen(X);
    int n = strlen(Y);

    int L[m + 1][n + 1];

    // Build DP table
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = 1 + L[i - 1][j - 1];
            else
                L[i][j] = (L[i - 1][j] > L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1];
        }
    }

    // Print DP Table
    printf("\n===== DP Length Table L[i][j] =====\n");
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            printf("%3d ", L[i][j]);
        }
        printf("\n");
    }

    printf("\nLCS Length = %d\n", L[m][n]);

    // Reconstruct LCS
    int index = L[m][n];
    char lcs[index + 1];
    lcs[index] = '\0';

    int i = m, j = n;

    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs[index - 1] = X[i - 1];
            i--;
            j--;
            index--;
        }
        else if (L[i - 1][j] > L[i][j - 1])
            i--;
        else
            j--;
    }

    printf("One Valid LCS Sequence = %s\n", lcs);

    printf("\nTime Complexity: O(m * n)\n");
}

int main() {
    char X[100], Y[100];

    printf("==============================================\n");
    printf(" LCS for Task Scheduling in Cloud Environment\n");
    printf(" Domain: Cloud Task Scheduling using Priority Queues\n");
    printf("==============================================\n");

    // Input Task Sequence 1
    do {
        printf("\nEnter Task Sequence X (example: T1_T3_T5): ");
        scanf("%s", X);
        if (!isValid(X))
            printf("Invalid input! Only alphabets, digits, and '_' allowed. Try again.\n");
    } while (!isValid(X));

    // Input Task Sequence 2
    do {
        printf("Enter Task Sequence Y (example: T2_T3_T5): ");
        scanf("%s", Y);
        if (!isValid(Y))
            printf("Invalid input! Only alphabets, digits, and '_' allowed. Try again.\n");
    } while (!isValid(Y));

    computeLCS(X, Y);

    return 0;
}

