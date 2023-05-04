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

    vector<int> node2pe(arc.size(),-1);

    for(auto &e:G.edges()){
        /* get edge */
        int u = e.first;
        int v = e.second;

        /* find edge case */
        int is_ualloc = (node2pe[u] == -1 ? 0:2);
        int is_valloc = (node2pe[v] == -1 ? 0:1);
        int edge_case = is_ualloc + is_valloc;

        /*
            alloc:
                dado um nó u, alloca ele em um pe vazio
                se nenhum pe vazio foi encontrado:
                    -- não pode ocorrer (número de nós <= número de pes)
        */
       int peU, peV;

        switch (edge_case){
        case 0: // u and v dealloc
            break;
        
        case 1: // u dealloc and v alloc
            break;
        
        case 2: // u alloc and v dealloc
            break;

        case 3: // u and v alloc
            break;

        default:
            break;
        }
    }

}

void Solution::local_search(){}