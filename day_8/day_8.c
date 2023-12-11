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


        num_read_nodes++;
    }





    
    int n_steps = 0;
    char ch = route[0];
    int n_rule = 0;
    Node node = nodes[nodes_index[0]];//Always will be AAA
    while(!is_end_node(node)){
        
        int index;
        if(ch == 'L'){
            index = node_to_index(node.left);
        } else {// ch == 'R'
            index = node_to_index(node.right);
        }


        node = nodes[nodes_index[index]];
        

        n_rule = (n_rule + 1) % num_rules;
        ch = route[n_rule];
        n_steps++;
    }

    printf("NUMBER OF STEPS: %d\n",n_steps);

    

    free(route);
    free(line);
    fclose(file);
}