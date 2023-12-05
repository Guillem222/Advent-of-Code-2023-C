#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>



#define MAX_NUM_SEEDS 50
#define SIZE_MAP_ARR 100
#define NUM_OF_MAPS 7

const char* FILE_NAME = "input_file.txt";


// Global structures
unsigned long map_rules[NUM_OF_MAPS][SIZE_MAP_ARR][3];
unsigned long seeds_id[MAX_NUM_SEEDS][2];
int num_rules[NUM_OF_MAPS];
unsigned long* min_location;



// Fills a map array with the mapping rules, returns the number of rules
int fill_map(unsigned long rules[SIZE_MAP_ARR][3], FILE *file){
    char* line = NULL;
    size_t len = 0;
    int read;

    int num_rules = 0;

    // Read title line.
    read = getline(&line, &len, file);

    // Read the rules
    while ((read = getline(&line, &len, file)) != -1) {
        
        
        const char del[2] = " ";
        char* token = strtok(line,del);
        int i_tok = 0;

        while(token != NULL){


            rules[num_rules][i_tok++] = strtoul(token,NULL,10);

            
            token = strtok(NULL,del);
        }

        

        if(line[0] == '\n'){
            break;
        }   
        
        num_rules++;


    }

    return num_rules;
}



void *search_min_location(void *arg){

    int s_id = *((int *) arg);

    unsigned long min = -1;//max unsigned long
    for (unsigned long curr_seed = seeds_id[s_id][0]; curr_seed < (seeds_id[s_id][0] + seeds_id[s_id][1]); curr_seed++){
            
        unsigned long current_measure = curr_seed;

        for (int j = 0; j < NUM_OF_MAPS; j++){

                // Find mapping
                int k_rule = 0;
                while( !((current_measure >= map_rules[j][k_rule][1]) &&
                        (current_measure < (map_rules[j][k_rule][1] + map_rules[j][k_rule][2]))) && (k_rule < num_rules[j]) ){
                    k_rule++;
                }

                // if mapping found
                if(k_rule < num_rules[j]){
                    // current - source + destination
                    current_measure = (current_measure - map_rules[j][k_rule][1]) + map_rules[j][k_rule][0];
                }

        }

        if(current_measure < min){
            min = current_measure;
        }
            


    }

    min_location[s_id] = min;
}


int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }


    size_t len = 0;
    int read;
    char* line = NULL;

    // Get seeds id
    
    int num_seed_info = 0;
    
    read = getline(&line, &len, file);

    char* seeds_str = strchr(line,':') + 2;
    const char del[2] = " ";
    char* token = strtok(seeds_str,del);
    int tokens_read = 0;

    while(token != NULL){

        seeds_id[tokens_read / 2][tokens_read % 2] = strtoul(token,NULL,10);

        tokens_read++;

        token = strtok(NULL,del);
    }
    num_seed_info = tokens_read / 2;// euclidean div


    // Read breakline '\n'
    read = getline(&line, &len, file);


    // Map rules filling
    
    
    

    for (int i = 0; i < NUM_OF_MAPS; i++){
        num_rules[i] = fill_map(map_rules[i],file);
    }
    
    
    min_location = (unsigned long*) malloc(num_seed_info * sizeof(unsigned long));
    int* ids_info = (int*) malloc(num_seed_info * sizeof(int));
    for(int i = 0; i < num_seed_info;i++){
        ids_info[i] = i;
    }
    
    thrd_t* tid = (thrd_t*) malloc(num_seed_info * sizeof(thrd_t));

    for (int s_id = 0; s_id < num_seed_info; s_id++){
        
        printf("Seed_info: %d\n",s_id);
        thrd_create(&tid[s_id],(thrd_start_t) search_min_location,(void *)&ids_info[s_id]);
        
    }

    for (int s_id = 0; s_id < num_seed_info; s_id++){
        thrd_join(tid[s_id], NULL);
        printf("Thread %d finished\n",s_id);
    }

    unsigned long min_val = -1;//max val
    for(int s_id = 0; s_id < num_seed_info; s_id++){
        if(min_location[s_id] < min_val){
            min_val = min_location[s_id];
        }
    }

    printf("MIN LOCATION: %lu\n",min_val);
    
    
    free(tid);
    free(ids_info);
    free(min_location);
    free(line);
    fclose(file);

}
