#include <stdio.h>

// Function to get maximum of two numbers
float max(float a, float b) {
    return (a > b) ? a : b;
}

// Structure for item
typedef struct {
    int weight;
    int value;
} Item;

// Function to sort items by value/weight ratio
void sortItems(Item items[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = i+1; j < n; j++) {
            float r1 = (float)items[i].value / items[i].weight;
            float r2 = (float)items[j].value / items[j].weight;
            if(r1 < r2) {
                Item temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }
        }
    }
}

// Fractional Knapsack function
float fractionalKnapsack(int capacity, Item items[], int n) {
    sortItems(items, n);
    float totalValue = 0;
    for(int i = 0; i < n; i++) {
        if(capacity >= items[i].weight) {
            capacity -= items[i].weight;
            totalValue += items[i].value;
        } else {
            totalValue += (float)items[i].value * capacity / items[i].weight;
            break;
        }
    }
    return totalValue;
}

int main() {
    int n, capacity;
    printf("Enter number of items: ");
    scanf("%d", &n);

    Item items[n];
    for(int i = 0; i < n; i++) {
        printf("Enter value and weight for item %d: ", i+1);
        scanf("%d %d", &items[i].value, &items[i].weight);
    }

    printf("Enter knapsack capacity: ");
    scanf("%d", &capacity);

    float maxVal = fractionalKnapsack(capacity, items, n);
    printf("Maximum value in knapsack = %.2f\n", maxVal);

    return 0;
}

