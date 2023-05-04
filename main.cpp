#include "include/solution.h"

int main(){

    /* INIT SOLUTION */
    string json_file = "misc/default_arch128x2.json";
    string dot_file  = "misc/debug.dot";
    
    Solution solveby(json_file, dot_file);
    
    /* GREEDY INITIAL SOLUTION */
    solveby.greedy();

    /* LOCAL SEARCH */
    solveby.local_search();

    /* METAHEURISTIC */

    return 0;
}