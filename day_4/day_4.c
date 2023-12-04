#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE_WIN_NUM 20
#define SIZE_CHECK_NUM 100

const char* FILE_NAME = "input_file.txt";


int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }


    size_t len = 0;
    int read;
    char* line = NULL;

    int w_nums_arr[SIZE_WIN_NUM];
    int c_nums_arr[SIZE_CHECK_NUM];

    int total_sum = 0;


    while ((read = getline(&line, &len, file)) != -1) {
        
        // Separate wining numbers of numbers to check
        char* w_nums = strchr(line,':') + 2;
        char* c_nums = strchr(line,'|') - 1;
        *c_nums = '\0';
        c_nums += 3;

        int w_ind = 0;
        int c_ind = 0;
       

        // Tokenize wining numbers
        const char del[2] = " ";
        char* token = strtok(w_nums,del);

        while(token != NULL){

            w_nums_arr[w_ind++] = atoi(token);

            token = strtok(NULL,del);
        }


        // Tokenize check numbers
        token = strtok(c_nums,del);

        while(token != NULL){

            c_nums_arr[c_ind++] = atoi(token);

            token = strtok(NULL,del);
        }

        double points = 0.5;
        for(size_t i = 0; i < w_ind; i++){

            for(size_t j = 0; j < c_ind; j++){
                if(w_nums_arr[i] == c_nums_arr[j]){
                    points *= 2;
                }
                
            }
        }

        total_sum += (int) points;

    }

    printf("TOTAL SUM: %d\n",total_sum);


    free(line);
    fclose(file);

}