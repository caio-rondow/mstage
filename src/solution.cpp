#include "../include/solution.h"

Solution::Solution(const string&json_file, const string&dot_file, int copy, int extra){

    /* LOAD CONFIG. FILE AND GRAPH */
    DataLoader loader;
    G   = loader.read_dot(dot_file, copy);
    arc = loader.read_json(json_file);

    /* CREATE OMEGA NETWORK */
    Omega net(N,STAGE,extra);

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            visited[i][j] = -1;
        }
    }
}

void Solution::clear(){

    net.clear();
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            visited[i][j] = -1;
        }
    }  
}

int Solution::size() const{ return arc.size(); }

int Solution::get_optimum() const{ return G.number_of_edges(); }

void Solution::info() const{
    cout << "------------------------------------------------\n";
    cout << "Graph and Arquitecture:\n";
    cout << "\tnumber of nodes:\t" << G.number_of_nodes() << "\n";
    cout << "\tnumber of edges:\t" << G.number_of_edges() << "\n";
    cout << "\tnumber of pes  :\t" << arc.size()          << "\n";
    cout << "\toptimum        :\t" << get_optimum()       << "\n\n";

    cout << "Network:\n";
    cout << "\tnumber of input/output:\t" << N << "\n";
    cout << "\tnumber of stages      :\t" << STAGE << "\n";
    cout << "\tnumber of extra stages:\t" << EXTRA << "\n";
    cout << "------------------------------------------------\n\n";
}

int Solution::greedy(vector<int>&solution, const string&search){

    if(arc.size()<G.number_of_nodes()){
        cerr << "This graph does not fit into the architecture.\n";
        cerr << "graph has " << G.number_of_nodes() << " nodes and arch. has " << arc.size() << " cells.\n";
        exit(1);
    }

    vector<int> pe2node(arc.size(),-1);
    vector<pair<int,int>> edges;
    vector<int> fail;

    int num_pes = arc.size();

    if(search=="bfs"){
        edges = G.bfs_edges();
    } else if(search=="dfs"){
        edges = G.dfs_edges();
    } else{
        edges = G.edges();
    }

    for(auto &e:G.edges()){

        int node_i = e.first;
        int node_j = e.second;
        bool routed=false;
        int pei,pej;
        
        int is_pe_i_alloc = (solution[node_i] == -1 ? 0 : 1);
        int is_pe_j_alloc = (solution[node_j] == -1 ? 0 : 2);

        int route_case = is_pe_i_alloc + is_pe_j_alloc;
    
        // self loop
        if(node_i == node_j){

            if(route_case == 0){

                for(int pei=0; pei<num_pes; pei++){
                    if(pe2node[pei]==-1 && _route(pei,pei) != FAIL){
                        solution[node_i] = pei;
                        pe2node[pei] = node_i;
                    }
                }

            } else{
                _route(solution[node_i], solution[node_i]);
            }

        } else{
            
            switch (route_case){
                case 0: // ambos node_i e node_j não foram alocados
                    
                    for(int pei=0; pei<num_pes; pei++){
                        
                        if(pe2node[pei]!=-1){
                            continue;
                        }

                        for(int pej=0; pej<num_pes; pej++){

                            if( pei != pej && pe2node[pej]==-1 && _route(pei,pej) != FAIL){

                                solution[node_i] = pei;
                                solution[node_j] = pej;

                                pe2node[pei] = node_i;
                                pe2node[pej] = node_j;

                                routed = true;
                                break;
                            }
                        }
                        if(routed)
                            break;
                    }

                break;
                
                case 1: // somente node_i foi alocado

                    pei = solution[node_i];

                    for(int pej=0; pej<num_pes; pej++){
                        
                        if(pe2node[pej]==-1 && _route(pei,pej) != FAIL){

                            solution[node_j] = pej;
                            pe2node[pej] = node_j;
                            break;
                        }
                    }

                break;
                
                case 2: // somente node_j foi alocado

                    pej = solution[node_j];

                    for(int pei=0; pei<num_pes; pei++){

                        if(pe2node[pei]==-1 && _route(pei,pej) != FAIL){
                            
                            solution[node_i] = pei;
                            pe2node[pei] = node_i;
                            break;
                        }
                    }

                break;
                
                default: // ambos node_i e node_j foram alocados

                    pei = solution[node_i];
                    pej = solution[node_j];
                    _route(pei,pej);

                break;
            }
        }
    }

    // criando solução completa
    for(int i=0; i<num_pes; i++){
        bool failed=true;
        for(int j=0; j<num_pes; j++){
            if(i == solution[j]){
                failed=false;
                break;
            }
        }
        if(failed){
            fail.push_back(i);
        }
    }

    int k=0;
    for(int i=0; i<num_pes; i++){
        if(solution[i]==-1){
            solution[i] = fail[k++];
        }
    }

    int current_cost = evaluate_initial_solution(solution);

    return current_cost;
}

int Solution::evaluate_initial_solution(const vector<int>&solution){

    if(arc.size() < G.number_of_nodes()){
        cerr << "least PE's them nodes.\n";
        return 0;
    }

    int cost = 0;
    clear();
    
    for(auto &node:G.nodes()){
        cost+=_add_node_into_pe(solution, node, solution[node]);
    }

    return cost;
}

int Solution::_route(int peu, int pev){

    for(const int &input:arc.oport(peu)){
        for(const int &output:arc.iport(pev)){
            int word = net.route(input, output);
            if(word>=0){
                return word;
            }
        }
    }
    return FAIL;
}

int Solution::_add_node_into_pe(const vector<int>&solution, int node, int pe){

    if(node < 0 || node >= G.number_of_nodes()){
        return 0;
    }

    int added=0;
    for(const int &predecessor:G.predecessors(node)){
        
        if(visited[predecessor][node]<0){

            int word = _route(solution[predecessor], pe);
            if(word != FAIL){
                visited[predecessor][node] = word;
                added++;
            }
        } 
    }

    for(const int &neighbor:G.neighbors(node)){

        if(visited[node][neighbor]<0){

            int word = _route(pe, solution[neighbor]);
            if(word != FAIL){
                visited[node][neighbor] = word;
                added++;
            }
        } 
    }

    return added;
}

int Solution::_remove_node_from_pe(const vector<int>&solution, int node, int pe){

    if(node<0 || node>=G.number_of_nodes())
        return 0;

    int removed=0;

    for(const int &predecessor:G.predecessors(node)){
        
        int word = visited[predecessor][node];

        if(net.unroute(word)){
            removed++;
            visited[predecessor][node] = FAIL;
        }  
    }

    for(const int &neighbor:G.neighbors(node)){
        
        int word = visited[node][neighbor];

        if(net.unroute(word)){
            visited[node][neighbor] = FAIL;
            removed++;
        } 
    }

    return removed;
}

int Solution::_swap_two_nodes(vector<int>&solution, int current_cost, int node_i, int node_j){

    if(node_i < 0 || node_j < 0|| node_i >= arc.size() || node_j >= arc.size()){
        cerr << "NODE OUT OF RANGE: " << node_i << " " << node_j << "\n";
        exit(1);
    }

    int removed=0;
    int added=0;

    if(node_i != node_j){

        int pei = solution[node_i];
        int pej = solution[node_j];

        removed += _remove_node_from_pe(solution, node_i, pei);
        removed += _remove_node_from_pe(solution, node_j, pej);
        swap(solution[node_i], solution[node_j]);
        added += _add_node_into_pe(solution, node_i, pej);
        added += _add_node_into_pe(solution, node_j, pei);
    }

    return (current_cost - removed + added);
}

int Solution::local_search(vector<int>&solution, int cost){   

    int num_pes = arc.size();
    int current_cost = cost;
    bool is_improving=true;
    int best_cost = current_cost;

    while( is_improving && current_cost!=G.number_of_edges() ){

        is_improving = false;
        
        for(int i=0; i<num_pes; i++){
            for(int j=i+1; j<num_pes; j++){

                int new_cost = _swap_two_nodes(solution, current_cost, i, j);
                
                if(new_cost > G.number_of_edges()){
                    cerr << "erro\n";
                    exit(1);
                }

                if(new_cost > best_cost){
                    current_cost = new_cost;
                    is_improving = true;
                    best_cost = new_cost;
                }
                else{
                    current_cost = _swap_two_nodes(solution, new_cost, i, j);
                }
            }
        }
    }

    return best_cost;
}

double Solution::acceptance_probability(int deltaC, double temp) const{
    return exp(deltaC/temp);
}

int Solution::annealing(vector<int>&solution, int cost){

    /* https://stackoverflow.com/questions/9878965/rand-between-0-and-1 */
    mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    uniform_real_distribution<double> unif(0, 1);
    // initialize a uniform distribution between 0 and nodes-1
    uniform_int_distribution<int> iunif(0, G.number_of_nodes()-1);

    // int MAX_SWAPS = ((log10(MIN_TEMPERATURE) - log10(MAX_TEMPERATURE)) / log10(DECAY)) + 1;
    // int *nodes_to_swap = new int[2*MAX_SWAPS];
    // nodes_to_swap[0] = iunif(rng); 
    // for(int i=1; i<2*MAX_SWAPS; i++){
    //     do{
    //         nodes_to_swap[i] = iunif(rng);
    //     } while(nodes_to_swap[i] == nodes_to_swap[i-1]);
    // }
    // int k=0;

    double temp = MAX_TEMPERATURE;
    int current_cost = cost;
    int node_i, node_j;

    while(temp>=MIN_TEMPERATURE && current_cost!=G.number_of_edges()){
        
        node_i = iunif(rng);
        do{
            node_j=iunif(rng);
        } while(node_j==node_i);

        int new_cost = _swap_two_nodes(solution, current_cost, node_i, node_j);
        int deltaC = new_cost - current_cost;

        if(new_cost > G.number_of_edges()){
            cerr << "erro\n";
            exit(1);
        }

        double random_value = unif(rng);

        if(deltaC>0 || random_value < acceptance_probability(deltaC, temp)){
            current_cost = new_cost;
        } else{
            current_cost = _swap_two_nodes(solution, new_cost, node_i, node_j);
        }

        temp *= DECAY;
    }
 
    // delete[] nodes_to_swap;

    return current_cost;
}