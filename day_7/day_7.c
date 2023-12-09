#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_TYPE_CARDS 13
#define MAX_NUM_HANDS 1000
#define NUM_TYPES 7

#define HIGH_CARD 0
#define ONE_PAIR 1
#define TWO_PAIR 2
#define THREE_KIND 3
#define FULL_HOUSE 4
#define FOUR_KIND 5
#define FIVE_KIND 6


const char* FILE_NAME = "input_file.txt";
int num_type_cards[NUM_TYPES];
char HANDS[NUM_TYPES][MAX_NUM_HANDS][5 + 1];//...'\0'
int BIDS[NUM_TYPES][MAX_NUM_HANDS];




//2,3,4,5,6,7,8,9,T,J,Q,K,A
int card_to_ind(char card){
    if(card >= '0' && card <= '9'){
        return card - '0' - 2;
    } 
    else if(card == 'T') return 8;
    else if(card == 'J') return 9;
    else if(card == 'Q') return 10;
    else if(card == 'K') return 11;
    else if(card == 'A') return 12;
    else return -1;
}


int get_hand_type(char* hand){
    // Count repeated cards
    int count_arr[NUM_TYPE_CARDS];
    memset(count_arr,0,sizeof(count_arr));
    for(int i = 0; i < 5; i++){
        count_arr[card_to_ind(hand[i])]++;
    }


    int count_t_cards[6];
    memset(count_t_cards,0,sizeof(count_t_cards));
    for(int i = 0; i < NUM_TYPE_CARDS; i++){
        if(count_arr[i] > 1){
            count_t_cards[count_arr[i]]++;
        }
    }

    if(count_t_cards[5] > 0) return FIVE_KIND;
    if(count_t_cards[4] > 0) return FOUR_KIND;
    if((count_t_cards[3] > 0) && (count_t_cards[2] > 0)) return FULL_HOUSE;
    if(count_t_cards[3] > 0) return THREE_KIND;
    if(count_t_cards[2] > 1) return TWO_PAIR;
    if(count_t_cards[2] > 0) return ONE_PAIR;
    return HIGH_CARD;

}


int is_higher_rank(char hand_1[6],char hand_2[6]){
    for(int i = 0; i < 5 ; i++){
        int card_1 = card_to_ind(hand_1[i]);
        int card_2 = card_to_ind(hand_2[i]);
        if(card_1 > card_2){
            return 1;
        } else if(card_2 > card_1){
            return 0;
        }
    }

    return 0;
}


/*
    EXTRACTED FROM https://www.geeksforgeeks.org/bubble-sort/
*/
void swap_bids(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}


void swap_hands(char *str1, char *str2) {

    char temp[6];

    strncpy(temp, str1, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    strncpy(str1, str2, sizeof(temp) - 1);
    str1[sizeof(temp) - 1] = '\0';

    strncpy(str2, temp, sizeof(temp) - 1);
    str2[sizeof(temp) - 1] = '\0';
}
 
/*
    EXTRACTED FROM https://www.geeksforgeeks.org/bubble-sort/
    (modified)
*/
void bubbleSort(char hands[MAX_NUM_HANDS][5+1],int bids[MAX_NUM_HANDS], int n)
{
    int i, j;
    char swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (is_higher_rank(hands[j],hands[j + 1])) {
                swap_hands(hands[j],hands[j + 1]);
                swap_bids(&bids[j], &bids[j + 1]);
                swapped = 1;
            }
        }
 
        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == 0)
            break;
    }
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



    
    memset(num_type_cards,0,sizeof(num_type_cards));
    
    int num_hands = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        
        // Get bid and rank strings
        char* bid_str = strchr(line,' ');
        bid_str[0] = '\0';
        bid_str += 1;
        char* rank_str = line;


        //Get Hand type
        int hand_type = get_hand_type(rank_str);

        //Store hand
        strcpy(HANDS[hand_type][num_type_cards[hand_type]],rank_str);


        //Store bid amount
        BIDS[hand_type][num_type_cards[hand_type]] = atoi(bid_str);


        num_type_cards[hand_type]++;
        num_hands++;

    }

    //Sort each hand type array.
    
    for(int i = 0; i < NUM_TYPES; i++){
        bubbleSort(HANDS[i],BIDS[i],num_type_cards[i]);
    }
    
    int total_winings = 0;
    int num_rank_pos = 1;
    for(int i = 0; i < NUM_TYPES; i++){
        for(int j = 0; j < num_type_cards[i];j++){
            
            total_winings += BIDS[i][j] * num_rank_pos;
            num_rank_pos++;
        }
    }

    printf("TOTAL WININGS: %d\n",total_winings);

    free(line);
    fclose(file);
}