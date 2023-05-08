#include "include/solution.h"

int main(int argc, char **argv){

    /* INIT SOLUTION */
    string json_file = "misc/arch/default_arch128x2.json";
    string dot_file  = argv[1];
    int copy         = atoi(argv[2]);

    cout << dot_file << ", " << copy << ", ";
    
    Solution solveby(json_file, dot_file, copy);
    
    /* GREEDY INITIAL SOLUTION */
    solveby.greedy();
    
    /* LOCAL SEARCH */
    solveby.local_search();

    /* METAHEURISTIC */

    return 0;
}