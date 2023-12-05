#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_NUM_SEEDS 50
#define SIZE_MAP_ARR 100
#define NUM_OF_MAPS 7

const char* FILE_NAME = "input_file.txt";

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
    unsigned long seeds_id[MAX_NUM_SEEDS];
    int num_seeds = 0;
    
    read = getline(&line, &len, file);

    char* seeds_str = strchr(line,':') + 2;
    const char del[2] = " ";
    char* token = strtok(seeds_str,del);

    while(token != NULL){

        seeds_id[num_seeds++] = strtoul(token,NULL,10);

        token = strtok(NULL,del);
    }

    // Read breakline '\n'
    read = getline(&line, &len, file);


    // Map rules filling
    unsigned long map_rules[NUM_OF_MAPS][SIZE_MAP_ARR][3];
    
    int num_rules[NUM_OF_MAPS];

    for (int i = 0; i < NUM_OF_MAPS; i++){
        num_rules[i] = fill_map(map_rules[i],file);
    }
    

    unsigned long min_location = -1;//max unsigned long


    for (int i = 0; i < num_seeds; i++){

        unsigned long current_measure = seeds_id[i];

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


        if(current_measure < min_location){
            min_location = current_measure;
        }
        


    }

    printf("MIN LOCATION: %lu\n",min_location);
    
    
    


    free(line);
    fclose(file);

}