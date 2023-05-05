#include "../include/solution.h"

Solution::Solution(const string&json_file, const string&dot_file){

    /* LOAD CONFIG. FILE AND GRAPH */
    DataLoader loader;
    G   = loader.read_dot(dot_file);
    arc = loader.read_json(json_file);

    /* CREATE OMEGA NETWORK */
    Omega net(256,4,1);
}

void Solution::_alloc_node(vector<int>&node2pe,vector<int>&pe2node,int num_pes, int u, int v){
    
    int peU, peV;
    for(int i=0; i<num_pes; i++){
        
        /* select a free peU */
        if(node2pe[u] == -1){
            if(pe2node[i] != -1)
                continue;
            peU = i;
        } else{
            peU = node2pe[u];
        }

        for(int j=0; j<num_pes; j++){

            /* select a free peV */
            if(node2pe[v] == -1){
                if(pe2node[j] != -1 || i == j)
                    continue;
                peV = j;
            } else{
                peV = node2pe[v];
            }

            /* test all route connections */
            for(const int &pout:arc.oport(peU)){        
                for(const int &pin:arc.iport(peV)){
                    if(net.route(pout, pin)){
                        /* mark pe as used */
                        node2pe[u] = peU;
                        node2pe[v] = peV;
                        pe2node[peU] = u;
                        pe2node[peV] = v;
                        return;
                    }
                }
            }
        }
    }
}

vector<int> Solution::greedy(){

    int num_pes = arc.size();
    vector<int> node2pe(num_pes,-1);
    vector<int> pe2node(num_pes,-1);

    for(auto &e:G.edges()){
        /* get edge */
        int u = e.first;
        int v = e.second;

        /* alloc node u and v to a pe */
        _alloc_node(node2pe,pe2node,num_pes,u,v);
    }

    /* DEBUG */
    for(auto &pe:node2pe)
        cout << pe << " ";
    cout << "\n";

    return node2pe;
}

void Solution::local_search(){}