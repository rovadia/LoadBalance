//Submitter Roei Ovadia
//ID 40885696

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define PROC_SIZE 100 //here decide number of processes

struct proc {
    int units; //like marbles
    int priority; //scheduling activity
    int left_neighbor; //left neighbor index
    int right_neighbor; //right neighbor index
};

int to_return;

void give_units (struct proc *current, struct proc *neighbor) { //used with giving from current to one neighbor
    double local_average = (current->units+neighbor->units)/2.0;
    int int_local_average = (int)local_average;
    if (local_average==int_local_average) { //if local_average is an integer value
        current->units = int_local_average; //then give both current and neighbor that number of units
        neighbor->units = int_local_average;
    }
    else { //if local_average has a .5
        current->units=int_local_average+1; //give the current processor half a unit above
        neighbor->units=int_local_average; //and the neighbor half a unit below
    }
}

void distribute (struct proc *current, struct proc *left, struct proc *right, double average) { //used with giving from current to both neighbors
    int total=current->units+left->units+right->units;
    int int_local_average = (int)average;

    current->units=int_local_average; //give everyone the average
    left->units=int_local_average;
    right->units=int_local_average;
    if (int_local_average==average) { //divides evenly by three?
        return; //we're done
    }
    else {
        while (total > current->units+left->units+right->units) { //if doesn't divide evenly by 3, keep looping (max once or twice) until it matches up
            current->units++;
        }
    }
}

int find_min_load_activity (struct proc *processor) { //find the minimum load activity and return the index of that processor
    int min_load_activity=INFINITY;
    int min_index=0;
    for (int i=0; i<PROC_SIZE;i++) { //for all processors
        if (processor[i].priority<min_load_activity) { //if the load activity is less than the minimum
            min_index=i; //min index is this index
            min_load_activity=processor[i].priority; //minimum found so far is this processor's priority
        }
    }
    return min_index;
}

void load_balance (struct proc *processor) {
    int current=find_min_load_activity(processor); //we will be load balancing the processor with minimum load activity
    int right=processor[current].right_neighbor;
    int left=processor[current].left_neighbor;
    double average = (processor[current].units+processor[left].units+processor[right].units)/3.0; //find average units of current, left, and right
    to_return=processor[current].priority; //this is number we will return when we are done, must set it to variable before we recalculate it

    processor[current].priority=rand()%1000+100+processor[current].priority; //give current processor a new scheduled time that is added on to its previous time

    if (processor[current].units<=average) { //if current processor has less or equal to units than the average
        return; //then it shouldn't give anything
    }

    else if (average<processor[right].units) { //if right neighbor has more units than the average
        give_units(&processor[current], &processor[left]); //only exchange with current and left
        return;
    }
    else if (average<processor[left].units ) { //if left processor has more units than the average
        give_units(&processor[current], &processor[right]); //only exchange with current and right
        return;
    }
    else if (average<processor[current].units) { //if current processor has more units than average (and left and right have less than average) means current processor has more than both
        //then we distribute amongst all three
        distribute(&processor[current],&processor[left],&processor[right],average);
        return;
    }
}

double percent_difference(double previous, double current) {
    //difference of two values divided by average of two values
    double percent_difference=(previous+current)/2;
    percent_difference=fabs(previous-current)/percent_difference;
    return percent_difference;
}


bool is_balanced (struct proc *processor) { //so we know when to stop loop
    double total=0;
    for (int i=0; i<PROC_SIZE; i++) { //calculate total
        total+=processor[i].units;
    }
    double average=total/PROC_SIZE; //calculate average load units
    for (int i=0; i<PROC_SIZE; i++) {
        if ( percent_difference(average, (double)processor[i].units) > 0.1  ) { //percent difference of any processor with average is above certain threshold?
            return false; //not done balancing yet
        }
    }
    return true; //all within that threshold, we are done balancing
}


int main() {
    struct proc processor [PROC_SIZE];
    for (int i=0; i<PROC_SIZE; i++) {
        processor[i].units=rand()%1000+10; //give each processor a random number of load units between 10 and 1000
        processor[i].priority=rand()%1000+100; //give each processor a random scheduling activity between 100 and 1000
        processor[i].left_neighbor=i-1; //give processor left and right neighbor
        processor[i].right_neighbor=i+1;
    }
    processor[0].left_neighbor=PROC_SIZE-1; //make circular
    processor[PROC_SIZE-1].right_neighbor=0;

    bool balanced=false;
    while (!balanced) { //continue load balancing until balanced
        load_balance(processor); //do load balance
        balanced=is_balanced(processor); //check if we can terminate
    }

    for (int i=0; i<PROC_SIZE; i++) {
        printf("units are %d\n",processor[i].units);
        printf("activity is %d\n",processor[i].priority);
        printf("\n");
    }

    printf("Total scheduling load activity is %d for %d processors\n", to_return, PROC_SIZE);


    FILE *f = fopen("C:\\Users\\rovad\\CLionProjects\\LoadBalance\\file1.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    int x;
    for (x=0;x<PROC_SIZE;x++) { //write averages to tab separated txt file, then use that for excel sheet
        fprintf(f, "%d\t", processor[x].units);
    }
    fclose(f);

    return 0;
}