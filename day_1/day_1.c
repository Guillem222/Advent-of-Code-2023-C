#include <stdio.h>
#include <string.h>

const char* FILE_NAME = "calibration_document.txt";

int main(){

    FILE* file = fopen(FILE_NAME,"r");

    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }

    int total_sum = 0;
    int first = -1;
    int last = -1;
    char ch = fgetc(file);
    while(ch != EOF){

        if(ch != '\n'){
            if(ch >= '0' && ch <= '9'){
                if(first == -1){
                    first = ch - '0';
                }
                last = ch - '0';             
            }
            
        } else {
            if(last != -1){
                total_sum += first * 10 + last;
                first = -1;
                last = -1;
            }
            
        }

        ch = fgetc(file);
        

    }

    // Sum the last line.
    if(last != -1){
        total_sum += first * 10 + last;
        first = -1;
        last = -1;
    }

    printf("TOTAL SUM: %d\n",total_sum);


    fclose(file);

}