#include "../include/solution.h"

Solution::Solution(const string&json_file, const string&dot_file){

    /* LOAD CONFIG. FILE AND GRAPH */
    DataLoader loader;
    G   = loader.read_dot(dot_file);
    arc = loader.read_json(json_file);

    /* CREATE OMEGA NETWORK */
    Omega net(256,4,1);
}

void Solution::greedy(){

    for(auto &e:G.edges()){
        cout << e.first << " " << e.second << "\n";   
    }

}

void Solution::local_search(){}