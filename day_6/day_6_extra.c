#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NUM_SIZE 50

const char* FILE_NAME = "input_file.txt";

// Compute how many ways can be the race beaten
// How many natural numbers follow the formula?
// Formula: 
//          distance < (time_race - time_pressed) * time_pressed 
//          0 < -distance + time_race * time_pressed - time_pressed^2
// Solve the quadratic equation and substract the ceiling integer solutions.
unsigned long calc_beat_n_ways(unsigned long time, unsigned long best_distance){

    long double sqrt_term = sqrtl( time * time + (-4) * (-1) * (-best_distance) );
    unsigned long tp_upper = (unsigned long) ceill(((-(long)time) -sqrt_term) / (-2)) ;
    unsigned long tp_lower = (unsigned long)(((-(long)time) +sqrt_term) / (-2)) + 1;//to ensure consistency

    return tp_upper - tp_lower;
}


int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }

    unsigned long time;
    unsigned long best_distance;
    char time_str[MAX_NUM_SIZE];
    time_str[0] = '\0';
    char best_distance_str[MAX_NUM_SIZE];
    best_distance_str[0] = '\0';

    size_t len = 0;
    int read;
    char* line = NULL;

    // Read times
    read = getline(&line, &len, file);

    char* times_str = strchr(line,':') + 2;
    const char del[2] = " ";
    char* token = strtok(times_str,del);

    int i = 0;
    while(token != NULL){

        strcat(time_str,token);

        token = strtok(NULL,del);
    }
    time = strtoul(time_str,NULL,10);
    printf("Time: %lu\n",time);

    
    // Read best distances
    read = getline(&line, &len, file);

    char* dist_str = strchr(line,':') + 2;
    token = strtok(dist_str,del);

    i = 0;
    while(token != NULL){

        strcat(best_distance_str,token);

        token = strtok(NULL,del);
    }
    best_distance = strtoul(best_distance_str,NULL,10);
    printf("Best distance: %lu\n",best_distance);

    

    
    unsigned long total_n_ways = calc_beat_n_ways(time,best_distance);


    printf("TOTAL PROD: %ld\n",total_n_ways);
    
    free(line);
    fclose(file);
}