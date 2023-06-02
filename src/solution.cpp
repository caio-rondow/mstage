#include "../include/solution.h"

Solution::Solution(const string&json_file, const string&dot_file, int copy){

    /* LOAD CONFIG. FILE AND GRAPH */
    DataLoader loader;
    G   = loader.read_dot(dot_file, copy);
    arc = loader.read_json(json_file);

    /* CREATE OMEGA NETWORK */
    Omega net(N,STAGE,EXTRA);
}

bool Solution::_alloc_node_into_pe(vector<int>&node2pe,vector<int>&pe2node,int num_pes, int u, int v){
    
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
                    if(net.route(pout, pin, peU, peV)){
                        /* mark pe as used */
                        node2pe[u] = peU;
                        node2pe[v] = peV;
                        pe2node[peU] = u;
                        pe2node[peV] = v;
                        return true;
                    }
                }
            }
        }
    }
    
    /* case where node are not alloc? */
    for(int i=0; i<num_pes; i++){
        
        if(node2pe[u]!=-1 && node2pe[v]!=-1)
            break;

        if(pe2node[i] == -1){            
            if(node2pe[u] == -1){
                pe2node[i] = u;
                node2pe[u] = i;
            } else if(node2pe[v] == -1){
                pe2node[i] = v;
                node2pe[v] = i;
            }
        }
    }

    return false;
}

vector<int> Solution::greedy(){

    if(arc.size()<G.number_of_nodes()){

        /* DEBUG TIRAR DEPOIS */
        cout << G.number_of_nodes() << ", ";
        cout << G.number_of_edges() << ", ";
        cout << arc.size() << ", ";
        cout << "0%\n";
        /* DEBUG TIRAR DEPOIS */

        cerr << "This graph does not fit into the architecture.\n";
        cerr << "graph has " << G.number_of_nodes() << " nodes and arch. has " << arc.size() << " cells.\n";
        exit(1);
    }

    int num_pes = arc.size();
    vector<int> node2pe(G.number_of_nodes(),-1);
    vector<int> pe2node(num_pes,-1);
    int counter=0;

    for(auto &e:G.edges()){
        /* get edge */
        int u = e.first;
        int v = e.second;

        /* alloc node u and v to a pe */
        bool ans = _alloc_node_into_pe(node2pe,pe2node,num_pes,u,v);
        if(ans){
            counter += 1;
        }
    }

    /* DEBUG TIRAR DEPOIS */
    // cout << G.number_of_nodes() << ", ";
    // cout << G.number_of_edges() << ", ";
    // cout << arc.size() << ", ";
    // cout << (100.0*counter)/G.number_of_edges() << "%\n";
    // cout << "FINAL COST GREEDY: " << counter << "\n";
    /* DEBUG TIRAR DEPOIS */

    return node2pe;
}

int Solution::_eval(vector<int>&S){

    Omega new_net(N,STAGE,EXTRA);
    int cost = 0;
    
    for(auto &edge:G.edges()){
        int u = edge.first;
        int v = edge.second;
        
        int peU = S[u];
        int peV = S[v];
        
        bool routed = false;

        /* test all route connections */
        for(const int &pout:arc.oport(peU)){        
            for(const int &pin:arc.iport(peV)){
                if(new_net.route(pout, pin, peU, peV)){
                    cost++;
                    routed = true;
                    break;
                }
            }
            if(routed)
                break;
        }
    }

    return cost;
}

vector<int> Solution::local_search(vector<int>&initial_solution){

    // init
    vector<int> S = initial_solution;
    int curr_cost = _eval(S);

    bool is_improved = true;
    int best_i = -1;
    int best_j = -1;
    int num_nodes = S.size();

    while(is_improved){

        is_improved = false;

        for(int i=0; i<num_nodes; i++){
            for(int j=i+1; j<num_nodes; j++){
               
                swap(S[i], S[j]);
                int new_cost = _eval(S);

                if(new_cost > curr_cost){
                    curr_cost = new_cost;
                    best_i = i;
                    best_j = j;
                    is_improved = true;
                }
                swap(S[i], S[j]);
            }
        }

        if(is_improved){
            swap(S[best_i], S[best_j]);
        }
    }

    cout << G.number_of_nodes() << ", ";
    cout << G.number_of_edges() << ", ";
    cout << arc.size() << ", ";
    cout << (100.0*curr_cost)/G.number_of_edges() << "%\n";

    return S;
}