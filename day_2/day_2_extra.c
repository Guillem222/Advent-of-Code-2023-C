#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* FILE_NAME = "input_file.txt";

#define RED 0
#define GREEN 1
#define BLUE 2


char COLOR_NAMES[3][10] = {"red","green","blue"};
int COLOR_MAX_VAL[3] = {12,13,14};

int get_color_ind(char* color){
    for (size_t i = 0; i < 3; i++){
        if(strcmp(COLOR_NAMES[i],color) == 0){
            return i;
        }
        
    }
    return -1;
    
}

int main(){

    FILE* file = fopen(FILE_NAME,"r");
    size_t len = 0;
    int read;
    if(file == NULL){
        printf("Can't open input file.\n");
        return -1;
    }

    char* line = NULL;
    int total_power = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        //printf("Line read from file: %s\n", line);

        // Get index value.
        char index_str[10];
        int index_len = strchr(line,':') - (line + 5); 
        strncpy(index_str,line + 5,index_len);
        index_str[index_len] = '\0';
        int index = atoi(index_str);
        //printf("%d\n",index);

        int color_max_val[3] = {0};
        // Tokenize line by ; separator
        char* outer_saveptr = NULL;
        char* inner_saveptr = NULL;

        const char del_outer[2] = ";";
        const char del_inner[2] = ",";
        char* token_semi = strtok_r(strchr(line,':') + 1,del_outer,&outer_saveptr);
        while(token_semi != NULL){

            // Tokenize outer token by , separator
            char* token_coma = strtok_r(token_semi,del_inner,&inner_saveptr);

            while(token_coma != NULL){

                char num_str[10];
                int num_len = strchr(token_coma + 1,' ') - token_coma + 1;
                strncpy(num_str,token_coma + 1,num_len);
                num_str[num_len] = '\0';
                int num_cubes = atoi(num_str);
                char color[10] = {'\0'};
                strcpy(color,strchr(token_coma + 1,' ') + 1);

                // Check if \n
                char* brk_l = strchr(color,'\n');
                if(brk_l != NULL){
                    *brk_l = '\0';
                }


                // Update fewest possible number of cubes possible (max)
                int color_index = get_color_ind(color);
                if(color_max_val[color_index] < num_cubes){
                    color_max_val[color_index] = num_cubes;
                }

                token_coma = strtok_r(NULL,del_inner,&inner_saveptr);
            }


            token_semi = strtok_r(NULL,del_outer,&outer_saveptr);
        }


        int power = color_max_val[0] * color_max_val[1] * color_max_val[2];
        total_power += power;
        
        


    }

    printf("TOTAL SUM: %d\n",total_power);


    

    free(line);
    fclose(file);
    
}