#include <stdio.h>
#include <string.h>


#define MAX_CALIBRATION_STR 256

const char* FILE_NAME = "calibration_document.txt";

//There is no "zero"
const char str_numbers[9][6] = {"one","two","three","four","five","six","seven","eight","nine"};


//Extracted from https://github.com/brgl/busybox/blob/master/libbb/strrstr.c
char *strrstr(const char *haystack, const char *needle) {
    char *last = NULL;
    char *found = strstr(haystack, needle);

    while (found != NULL) {
        last = found;
        found = strstr(found + 1, needle);
    }

    return last;
}

//It returns the index position of the first spelled digit on a string.
int first_spelled_digit(char calibr_str[],int* sp_num){
    //Highest possible pos
    char* min = calibr_str + MAX_CALIBRATION_STR;
    for (size_t i = 0; i < 9; i++){
        char* ptr = strstr(calibr_str,str_numbers[i]);
        if(ptr != NULL && ptr < min){
            min = ptr;
            *sp_num = i + 1;
        }
    }

    return min - calibr_str;
    
    
}

//It returns the index position of the first spelled digit on a string.
int last_spelled_digit(char calibr_str[],int* sp_num){
    //Lowest possible pos
    char* max = calibr_str;
    for (size_t i = 0; i < 9; i++){
        char* ptr = strrstr(calibr_str,str_numbers[i]);
        if(ptr != NULL && ptr > max){
            max = ptr;
            *sp_num = i + 1;
        }
    }

    return max - calibr_str;
    
    
}


int main(){

    FILE* file = fopen(FILE_NAME,"r");

    if(file == NULL){
        printf("Can't open calibration file.\n");
    }

    char calibr_str[MAX_CALIBRATION_STR];

    int total_sum = 0;
    int first, last;
    int pos_first = -1;
    int pos_last = -1;
    int current_pos = 0;
    char ch = fgetc(file);
    while(ch != EOF){

        calibr_str[current_pos] = ch;

        if(ch != '\n'){
            if(ch >= '0' && ch <= '9'){
                if(pos_first == -1){
                    first = ch - '0';
                    pos_first = current_pos;
                }
                last = ch - '0';
                pos_last = current_pos;       
            }
            
        } else {
            //Find first and last spelled digits.
            calibr_str[current_pos + 1] = '\0';
            int sp_num_first = -1;
            int sp_num_last = -1;

            int pos_sp_first = first_spelled_digit(calibr_str,&sp_num_first);
            int pos_sp_last = last_spelled_digit(calibr_str,&sp_num_last);


            if((sp_num_first != -1 && pos_sp_first < pos_first) || pos_first == -1){
                first = sp_num_first;
            }
            if(sp_num_last != -1 && pos_sp_last > pos_last || pos_last == -1){
                last = sp_num_last;
            }
            

            if(last != -1){
                total_sum += first * 10 + last;
                pos_first = -1;
                pos_last = -1;
                
            }
            
            current_pos = -1;
            
        }

        ch = fgetc(file);
        current_pos++;
        

    }

    // Sum the last line.
    //Find first and last spelled digits.
    calibr_str[current_pos + 1] = '\0';
    int sp_num_first = -1;
    int sp_num_last = -1;

    int pos_sp_first = first_spelled_digit(calibr_str,&sp_num_first);
    int pos_sp_last = last_spelled_digit(calibr_str,&sp_num_last);


    if((sp_num_first != -1 && pos_sp_first < pos_first) || pos_first == -1){
        first = sp_num_first;
    }
    if(sp_num_last != -1 && pos_sp_last > pos_last || pos_last == -1){
        last = sp_num_last;
    }
    

    if(last != -1){
        total_sum += first * 10 + last;
        pos_first = -1;
        pos_last = -1;
        
    }

    printf("TOTAL SUM: %d\n",total_sum);


    fclose(file);

}