#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQ_SIZE 50
#define MAX_DEPTH 50

const char* FILE_NAME = "input_file.txt";


//Checks if an int array is all zeros
int all_zeros(int* sequence, int size){
    for(int i = 0; i < size;i++){
        if(sequence[i] != 0) return 0;
    }
    return 1;
}

// Returns the prediction of the previous number of a sequence
//it assumes that curr_seq is not all zeroes.
int predict_prev_number(int* curr_seq, int size_curr_seq){
    int first_nums[MAX_DEPTH];
    int depth = 0;
    first_nums[depth++] = curr_seq[0];

    int* top_seq = (int*) malloc(size_curr_seq * sizeof(int));
    memcpy(top_seq,curr_seq,size_curr_seq * sizeof(int));

    int* below_seq = (int*) malloc((size_curr_seq - 1) * sizeof(int));
    int size_top_seq = size_curr_seq;

    while(1){

        
        for(int i = 1; i < size_top_seq; i++){
            below_seq[i - 1] = top_seq[i] - top_seq[i - 1];
        }

        first_nums[depth++] = below_seq[0];

        if(all_zeros(below_seq,size_top_seq - 1)){
            break;
        }

        free(top_seq);
        top_seq = below_seq;
        size_top_seq--;
        below_seq = (int*) malloc((size_top_seq - 1) * sizeof(int));
    }

    

    free(top_seq);
    free(below_seq);


    // Calc prediction
    int first_pred = 0;
    for(int i = (depth - 1); i >= 0;i--){
        first_pred = first_nums[i] - first_pred;
    }

    return first_pred;

}

int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open input file.\n");
        return -1;
    }
    size_t len = 0;
    int read;
    char* line = NULL;

    int total_sum_pred = 0;
    int curr_seq[MAX_SEQ_SIZE];
    int size_curr_seq; 


    const char del[2] = " ";
    char* token;
    while ((read = getline(&line, &len, file)) != -1) {

        size_curr_seq = 0;
        token = strtok(line,del);

        while(token != NULL){
            curr_seq[size_curr_seq++] = atoi(token);
            token = strtok(NULL,del);
        }

        total_sum_pred += predict_prev_number(curr_seq,size_curr_seq);

        
    }


    printf("TOTAL SUM PREDICTIONS: %d\n",total_sum_pred);

    free(line);
    fclose(file);
}