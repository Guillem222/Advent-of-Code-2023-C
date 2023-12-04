#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* FILE_NAME = "input_file.txt";


char* upper_line = NULL;
char* middle_line = NULL;
char* bottom_line = NULL;

int total_sum = 0;




int is_digit(char c){
    return (c >= '0' && c <= '9');
}


int calc_gear_ratio(int gear_pos){

    char check_matrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    int gear_ratio = 1;
    int num_parts = 0;

    //upper
    for(int i = -1; i < 2;i++){
        if((gear_pos + i) >= 0){

            if(is_digit(upper_line[gear_pos + i]) && (check_matrix[0][i + 1]) == 0){
                check_matrix[0][i + 1] = 1;

                char left[11];
                memset(left,'0',11);
                left[10] = '\0';
                left[9] = upper_line[gear_pos + i];
                int ind_left = 8;
                char right[11];
                memset(right,'\0',11);
                int ind_right = 0;
                
                //left
                int j = 1;
                if((gear_pos + i - j) >= 0){

                    while(is_digit(upper_line[gear_pos + i - j])){
                        
                        left[ind_left--] = upper_line[gear_pos + i - j];
                        // note already counted num
                        if((i - j) >= -1 && (i - j) <= 1){
                            check_matrix[0][i - j + 1] = 1;
                        }
                        j++;
                        if((gear_pos + i - j) < 0) break;
                    }   
                }
                
                


                //right
                j = 1;
                while(is_digit(upper_line[gear_pos + i + j])){
                    right[ind_right++] = upper_line[gear_pos + i + j];
                    // note already counted num
                    if((i + j) >= -1 && (i + j) <= 1){
                        check_matrix[0][i + j + 1] = 1;
                    }
                    j++;
                }

                strcat(left,right);
                gear_ratio *= atoi(left);
                num_parts++;






            }
        }
        
    }

    //middle

    
    if((gear_pos - 1) >= 0){
        //left
        if(is_digit(middle_line[gear_pos - 1])){

            char left[11];
            memset(left,'0',11);
            left[10] = '\0';
            left[9] = middle_line[gear_pos - 1];
            int ind_left = 8;
            int j = 1;
            if((gear_pos - 1 - j) >= 0){
                while(is_digit(middle_line[gear_pos - 1 - j])){
                    left[ind_left--] = middle_line[gear_pos - 1 - j];
                    j++;
                    if((gear_pos -1 - j) < 0) break;
                }
            }
            
            gear_ratio *= atoi(left);
            num_parts++;

        }
    }

    //right
     if(is_digit(middle_line[gear_pos + 1])){
        char right[11];
        memset(right,'\0',11);
        int ind_right = 0;
        right[ind_right++] = middle_line[gear_pos + 1];

        int j = 1;
        while(is_digit(middle_line[gear_pos + 1 + j])){
            right[ind_right++] = middle_line[gear_pos + 1 + j];
            j++;
        }

        gear_ratio *= atoi(right);
        num_parts++;

     }

    //bottom

    for(int i = -1; i < 2;i++){
        if((gear_pos + i) >= 0){
            if(is_digit(bottom_line[gear_pos + i]) && (check_matrix[2][i + 1]) == 0){
                check_matrix[2][i + 1] = 1;

                char left[11];
                memset(left,'0',11);
                left[10] = '\0';
                left[9] = bottom_line[gear_pos + i];
                int ind_left = 8;
                char right[11];
                memset(right,'\0',11);
                int ind_right = 0;

                //left
                int j = 1;
                if((gear_pos + i - j) >= 0){
                    while(is_digit(bottom_line[gear_pos + i - j])){
                        left[ind_left--] = bottom_line[gear_pos + i - j];
                        // note already counted num
                        if((i - j) >= -1 && (i - j) <= 1){
                            check_matrix[2][i - j + 1] = 1;
                        }
                        j++;
                        if((gear_pos + i - j) < 0) break;
                    }   
                }
                
                


                //right
                j = 1;
                while(is_digit(bottom_line[gear_pos + i + j])){
                    right[ind_right++] = bottom_line[gear_pos + i + j];
                    // note already counted num
                    if((i + j) >= -1 && (i + j) <= 1){
                        check_matrix[2][i + j + 1] = 1;
                    }
                    j++;
                }

                strcat(left,right);
                gear_ratio *= atoi(left);
                num_parts++;





            }
        }
        
    }


    if(num_parts != 2){
        return 0;
    } else {
        return gear_ratio;
    }



    
}


void check_gear(){
    size_t i = 0;
    char ch = middle_line[i];

    while(ch != '\n' && ch != '\0'){
        
        if(ch == '*'){
            total_sum += calc_gear_ratio(i);
        }   

        i++;
        ch = middle_line[i];
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
        
        

        // Check if gear in middle line
        check_gear();

        // Next lines.
        // Always same lenght
        memcpy(upper_line,middle_line,len);
        memcpy(middle_line,bottom_line,len);

    }

    
    memset(bottom_line,'.',len);
    bottom_line[len - 2] = '\n';
    bottom_line[len - 1] = '\0';

    //Process last line
    check_gear();
    printf("TOTAL SUM: %d\n",total_sum);


    free(upper_line);
    free(middle_line);
    free(bottom_line);
    fclose(file);
}