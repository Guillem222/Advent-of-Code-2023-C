#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NUM_NODE 65536
#define MAX_MAP_NODES 1000

const char* FILE_NAME = "input_file.txt";

typedef struct {
    char name[4];
    char left[4];
    char right[4];
} Node;

int nodes_index[MAX_NUM_NODE];//65536 bytes of memory
Node nodes[MAX_MAP_NODES];
int starting_nodes_index[MAX_MAP_NODES];

int is_end_node(Node node){
    return (node.name[0] == 'Z') && (node.name[1] == 'Z') && (node.name[2] == 'Z'); 
}

int node_to_index(char name[4]){
    int index = 0;
    index += (unsigned short) (name[0] - 'A');
    index = (index << 5);
    index += (unsigned short) (name[1] - 'A');
    index = (index << 5);
    index += (unsigned short) (name[2] - 'A');

    return index;
}

//Checks if all nodes of the current processed nodes array are final
int all_nodes_are_final(int* current_nodes_ind, int arr_size){

    for(int i = 0; i < arr_size;i++){
        Node node = nodes[current_nodes_ind[i]];
        if(node.name[2] != 'Z'){
            return 0;
        }
    }
    return 1;

}

//Applies a step to a node index and returns a node index.
int make_step(int node_index, char step){
    Node node = nodes[node_index];
    int index;
    if(step == 'L'){
        index = node_to_index(node.left);
    } else {// ch == 'R'
        index = node_to_index(node.right);
    }

    return nodes_index[index];
}

//Calc LCM of two numbers
/*
    EXTRACTED FROM https://www.geeksforgeeks.org/lcm-of-two-numbers-in-c/
*/
long long gcd(long long a, long long b) 
{ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
} 
/*
    EXTRACTED FROM https://www.geeksforgeeks.org/lcm-of-two-numbers-in-c/
*/
long long lcm(long long a, long long b) { return (a / gcd(a, b)) * b; }



int main(){
    FILE* file = fopen(FILE_NAME,"r");
    
    if(file == NULL){
        printf("Can't open calibration file.\n");
        return -1;
    }
    size_t len = 0;
    int read;
    char* route;
    
    memset(nodes_index,0,sizeof(nodes_index));

    int num_rules = getline(&route, &len, file) - 1;

    char* line = NULL;
    //Read whitespace
    read = getline(&line, &len, file);

    int num_starting_nodes = 0;
    int num_read_nodes = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        
        //Read node
        Node node;
        strncpy(node.name,line,3);
        node.name[3] = '\0';

        strncpy(node.left,line + 7,3);
        node.left[3] = '\0';

        strncpy(node.right,line + 12,3);
        node.right[3] = '\0';

        //Store node info
        nodes[num_read_nodes] = node;

        //Calc index
        int index = node_to_index(node.name);


        //Store index
        nodes_index[index] = num_read_nodes;

        //Check if starting node
        if(node.name[2] == 'A'){
            starting_nodes_index[num_starting_nodes++] = num_read_nodes;
        }


        num_read_nodes++;
    }





    
    // Compute how many cycle steps are in each path.
    int* current_nodes_ind = (int*) malloc(num_starting_nodes * sizeof(int));
    int* steps_for_cycle = (int*) malloc(num_starting_nodes * sizeof(int));
    for(int i = 0; i < num_starting_nodes;i++){
        current_nodes_ind[i] = starting_nodes_index[i];
    }

    
    for(int i = 0; i < num_starting_nodes; i++){
        int n_steps = 0;
        char ch = route[0];
        int n_rule = 0;

        int index = current_nodes_ind[i];
        Node node = nodes[index];
        while(node.name[2] != 'Z'){
            index = make_step(index,ch);
            node = nodes[index];

            n_rule = (n_rule + 1) % num_rules;
            ch = route[n_rule];
            n_steps++;
            
        }

        steps_for_cycle[i] = n_steps;
    }


    // Compute the lcm of each cycle steps.
    long long cycles_lcm = (long long) steps_for_cycle[0];
    for(int i = 1; i < num_starting_nodes;i++){
        cycles_lcm = lcm(cycles_lcm,(long long) steps_for_cycle[i]);
    }
    
    

    printf("NUMBER OF STEPS: %lld\n",cycles_lcm);

    free(steps_for_cycle);
    free(current_nodes_ind);
    free(route);
    free(line);
    fclose(file);
}