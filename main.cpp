#include "include/omega.h"
#include "include/digraph.h"
#include "include/architecture.h"
#include "include/dataloader.h"

int main(){

    /* LOAD CONFIG. FILE AND GRAPH */
    DataLoader loader;
    Architecture arc = loader.read_json("default_arch128x2.json");
    Digraph G        = loader.read_dot("debug.dot");

    /* CREATE OMEGA NETWORK */
    Omega net(256,4,1);

    int node2pe[arc.size()]; // which PE node i is
    int pe2node[arc.size()]; // which node PE i have
    
    /* GREEDY INITIAL SOLUTION */

    /* LOCAL SEARCH */

    /* METAHEURISTIC */

    return 0;
}