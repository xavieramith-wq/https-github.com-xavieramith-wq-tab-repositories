#include <stdio.h>
#define INF 999999

int dataCenterCount;                    // total data centers to monitor
int networkCost[20][20];                // latency/bandwidth cost matrix
int dp[20][1<<20];                      // DP memo table

// Bitmask DP for Cloud Monitoring Route Optimization (TSP)
int optimizeRoute(int currentDC, int visitedMask) {

    // If all datacenters are monitored, return cost to go back to Main Cloud HQ (DC-0)
    if(visitedMask == (1 << dataCenterCount) - 1)
        return networkCost[currentDC][0];

    // If already computed
    if(dp[currentDC][visitedMask] != -1)
        return dp[currentDC][visitedMask];

    int bestCost = INF;

    // Try visiting next unmonitored datacenter
    for(int nextDC = 0; nextDC < dataCenterCount; nextDC++) {

        // If this datacenter is NOT monitored yet
        if((visitedMask & (1 << nextDC)) == 0) {

            int newCost = networkCost[currentDC][nextDC] +
                           optimizeRoute(nextDC, visitedMask | (1 << nextDC));

            if(newCost < bestCost)
                bestCost = newCost;
        }
    }

    return dp[currentDC][visitedMask] = bestCost;
}

int main() {

    printf("Enter number of Cloud Data Centers to Monitor: ");
    scanf("%d", &dataCenterCount);

    // Validation check — domain specific
    if(dataCenterCount < 2 || dataCenterCount > 15) {
        printf("ERROR: Datacenter count must be between 2 and 15.\n");
        return 0;
    }

    printf("\nEnter Cloud Routing Cost Matrix (%d x %d):\n", dataCenterCount, dataCenterCount);
    printf("(Enter latency/bandwidth cost between datacenters)\n\n");

    // Read and validate matrix
    for(int i = 0; i < dataCenterCount; i++) {
        for(int j = 0; j < dataCenterCount; j++) {
            scanf("%d", &networkCost[i][j]);

            // Domain validations
            if(networkCost[i][j] < 0) {
                printf("ERROR: Routing cost/latency cannot be negative.\n");
                return 0;
            }
            if(i == j && networkCost[i][j] != 0) {
                printf("ERROR: Cost from a datacenter to itself must be 0.\n");
                return 0;
            }
        }
    }

    // Initialize DP table
    for(int i = 0; i < dataCenterCount; i++)
        for(int mask = 0; mask < (1 << dataCenterCount); mask++)
            dp[i][mask] = -1;

    // Start route optimization from Main HQ (datacenter 0)
    int minMonitoringCost = optimizeRoute(0, 1);

    printf("\nOptimal Cost to Monitor All Cloud Datacenters = %d\n", minMonitoringCost);

    return 0;
}

