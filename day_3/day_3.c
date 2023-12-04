#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* FILE_NAME = "input_file.txt";


char* upper_line = NULL;
char* middle_line = NULL;
char* bottom_line = NULL;

int total_sum = 0;




int is_symbol(char c){
    return !(c >= '0' && c <= '9') && c != '.' && c != '\n' && c != '\0';
}
// Check 8 1-d neighbours
//  1   if adjacent to a symbol
//  0   otherwise
int check_8_nb(size_t i){
    if(i > 0){
        if(is_symbol(upper_line[i - 1]) || is_symbol(middle_line[i - 1]) || is_symbol(bottom_line[i - 1])){
            return 1;
        }
    }
    if(is_symbol(upper_line[i]) || is_symbol(bottom_line[i])){
        return 1;
    }

    if(is_symbol(upper_line[i + 1]) || is_symbol(middle_line[i + 1]) || is_symbol(bottom_line[i + 1])){
            return 1;
    }

    return 0;

}


void check_part_number(){
    size_t i = 0;
    char ch = middle_line[i];

    char found = 0;
    size_t num_pos = 0;
    char current_number[11] = {'\0'};//maximum integer length + 1

    while(ch != '\n' && ch != '\0'){
        

        if(ch >= '0' && ch <= '9'){
            current_number[num_pos++] = ch;
            if(check_8_nb(i)){
                found = 1;
            }
        } else {
            if(found){
                int part_number = atoi(current_number);
                total_sum += part_number;        
            } 

            if(num_pos > 0){
                // Restart number
                num_pos = 0;
                memset(current_number,'\0',11);
                found = 0;
            }
            
        }

        i++;
        ch = middle_line[i];
    }

    if(found){
        int part_number = atoi(current_number);
        total_sum += part_number;        
    } 

}


int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }
    // Getline params
    size_t len = 0;
    int read;
    
    
    
    //Read first line
    if((read = getline(&middle_line, &len, file)) != -1){
        upper_line = (char *) malloc(len);
        memset(upper_line,'.',read);
        upper_line[read - 1] = '\n';
        upper_line[read] = '\0';
    } else {
        //There is nothing to read
        return 0;
    }



    
    
    while ((read = getline(&bottom_line, &len, file)) != -1) {
        
        

        // Sum the part numbers of the middle line.
        check_part_number();

        // Next lines.
        // Always same lenght
        memcpy(upper_line,middle_line,len);
        memcpy(middle_line,bottom_line,len);

    }

    
    memset(bottom_line,'.',len);
    bottom_line[len - 2] = '\n';
    bottom_line[len - 1] = '\0';

    //Process last line
    check_part_number();
    printf("TOTAL SUM: %d\n",total_sum);


    free(upper_line);
    free(middle_line);
    free(bottom_line);
    fclose(file);
}