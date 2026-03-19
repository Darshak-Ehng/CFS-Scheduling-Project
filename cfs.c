#include <stdio.h>
#include <unistd.h>

#define MAX 10

struct process {
    int pid;
    int bt;
    int remaining;
    float vruntime;
    int ct, tat, wt;
};

// Find process with minimum vruntime
int find_min(struct process p[], int n) {
    int i, min = -1;

    for(i = 0; i < n; i++) {
        if(p[i].remaining > 0) {
            if(min == -1 || p[i].vruntime < p[min].vruntime)
                min = i;
        }
    }
    return min;
}

// Print Gantt Chart
void print_gantt(int gantt[], int time) {
    int i;

    printf("\n\nGANTT CHART:\n");
    for(i = 0; i < time; i++) {
        printf("| P%d ", gantt[i]);
    }
    printf("|\n");

    printf("\nTIME LINE:\n");
    for(i = 0; i <= time; i++) {
        printf("%d   ", i);
    }
    printf("\n");
}

int main() {
    int n, i, time = 0, total = 0;
    int gantt[200];

    printf("===== CFS Scheduling (Linux Based) =====\n");

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct process p[MAX];

    // Input
    for(i = 0; i < n; i++) {
        printf("Enter Burst Time for P%d: ", i+1);
        scanf("%d", &p[i].bt);

        p[i].pid = i+1;
        p[i].remaining = p[i].bt;
        p[i].vruntime = 0;
        p[i].ct = 0;

        total += p[i].bt;
    }

    printf("\n--- Execution Starts ---\n\n");

    // CFS Execution (1 unit time slice)
    while(time < total) {
        int idx = find_min(p, n);

        printf("Time %d -> P%d running (vruntime=%.2f)\n",
               time, p[idx].pid, p[idx].vruntime);

        gantt[time] = p[idx].pid;

        // Execute for 1 unit
        p[idx].remaining--;
        p[idx].vruntime += 1.0;   // equal weight

        time++;
        sleep(1);   // animation

        if(p[idx].remaining == 0) {
            p[idx].ct = time;
            printf(">>> P%d Completed at time %d\n", p[idx].pid, time);
        }
    }

    // Calculate TAT and WT
    for(i = 0; i < n; i++) {
        p[i].tat = p[i].ct;       // AT = 0
        p[i].wt = p[i].tat - p[i].bt;
    }

    // Print Gantt Chart
    print_gantt(gantt, time);

    // Print Table
    printf("\nPROCESS DETAILS:\n");
    printf("PID\tBT\tCT\tTAT\tWT\n");

    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    return 0;
}
