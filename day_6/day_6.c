#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NUM_RACES 10

const char* FILE_NAME = "input_file.txt";

// Compute how many ways can be the race beaten
// How many natural numbers follow the formula?
// Formula: 
//          distance < (time_race - time_pressed) * time_pressed 
//          0 < -distance + time_race * time_pressed - time_pressed^2
// Solve the quadratic equation and substract the ceiling integer solutions.
int calc_beat_n_ways(int time, int best_distance){

    double sqrt_term = sqrt( time * time + (-4) * (-1) * (-best_distance) );
    int tp_upper = ceil((-time -sqrt_term) / (-2)) ;
    int tp_lower = (int)((-time +sqrt_term) / (-2)) + 1;//to ensure consistency
    return tp_upper - tp_lower;
}


int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }

    int times[MAX_NUM_RACES];
    int best_distances[MAX_NUM_RACES];
    int num_races = 0;

    size_t len = 0;
    int read;
    char* line = NULL;

    // Read times
    read = getline(&line, &len, file);

    char* times_str = strchr(line,':') + 2;
    const char del[2] = " ";
    char* token = strtok(times_str,del);


    while(token != NULL){

        times[num_races++] = atoi(token);

        token = strtok(NULL,del);
    }

    // Read best distances
    read = getline(&line, &len, file);

    char* dist_str = strchr(line,':') + 2;
    token = strtok(dist_str,del);

    int i = 0;
    while(token != NULL){

        best_distances[i++] = atoi(token);

        token = strtok(NULL,del);
    }



    int total_prod = 1;
    for(int i = 0; i < num_races;i++){
        total_prod *= calc_beat_n_ways(times[i],best_distances[i]);
    }

    printf("TOTAL PROD: %d\n",total_prod);

    free(line);
    fclose(file);
}