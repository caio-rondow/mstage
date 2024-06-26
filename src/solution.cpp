#include "../include/solution.h"

int64_t PackageItem(int word, int source, int target, int64_t validation){
    return word | source<<24 | (int64_t)target<<32 | validation<<40;
}


Solution::Solution(const string&json_file, const string&dot_file, int copy, int extra):
    stack(1000)
{

    /* LOAD CONFIG. FILE AND GRAPH */
    DataLoader loader;
    G   = loader.read_dot(dot_file, copy);
    arc = loader.read_json(json_file);

    /* CREATE OMEGA NETWORK */
    net = Omega(N,STAGE,extra);
    visited = vector<vector<int>>(N, vector<int>(N, -1));
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
    } else if(search=="seq"){
        edges = G.edges();
    } else{
        edges = G.edges();
        random_shuffle(edges.begin(),edges.end());
    }

    int current_cost=0;
    int fails=0;
    for(auto &e:edges){

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
                        current_cost++;
                        solution[node_i] = pei;
                        pe2node[pei] = node_i;
                    }
                }

            } else{
                if(_route(solution[node_i], solution[node_i]) != FAIL)
                    current_cost++;
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
                                current_cost++;
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
                            current_cost++;
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
                            current_cost++;
                            solution[node_i] = pei;
                            pe2node[pei] = node_i;
                            break;
                        }
                    }

                break;
                
                default: // ambos node_i e node_j foram alocados

                    pei = solution[node_i];
                    pej = solution[node_j];
                    if(_route(pei,pej) != FAIL){
                        current_cost++;
                    } else{
                        fails++;
                    }

                break;
            }
        }

        if(solution[node_i]==-1 || solution[node_j]==-1){
            fails++;
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

    current_cost = evaluate_initial_solution(solution);

    // cerr << fails << " " << current_cost2 << " " << current_cost << "\n";

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
        stack.clear();
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

                int64_t item = PackageItem(word, predecessor, node, 1);
                stack.push(item);
            }
        } 
    }

    for(const int &neighbor:G.neighbors(node)){

        if(visited[node][neighbor]<0){

            int word = _route(pe, solution[neighbor]);
            if(word != FAIL){
                visited[node][neighbor] = word;
                added++;

                int64_t item = PackageItem(word, node, neighbor, 1);
                stack.push(item);
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

            int64_t item = PackageItem(word, predecessor, node, 0);
            stack.push(item);
        }  
    }

    for(const int &neighbor:G.neighbors(node)){
        
        int word = visited[node][neighbor];

        if(net.unroute(word)){
            visited[node][neighbor] = FAIL;
            removed++;

            int64_t item = PackageItem(word, node, neighbor, 0);
            stack.push(item);
        } 
    }

    return removed;
}

void Solution::_undo_swap(){

    while(!stack.empty()){

        int64_t item = stack.top();
        stack.pop();

        /* unpack data */
        int word = (item & MAX_WORD);
        int source = ((item>>24) & MAX_SOURCE);
        int target = ((item>>32) & MAX_TARGET);
        int validation = ((item>>40) & MAX_V);

        if(validation){
            net.unroute(word);
            visited[source][target] = FAIL;
        } else{
            net.route(word);
            visited[source][target] = word;
        }
    }
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

    stack.clear();
    int num_pes = arc.size();
    int current_cost = cost;
    bool is_improving=true;

    while( is_improving && current_cost!=G.number_of_edges() ){

        is_improving = false;
        
        for(int i=0; i<num_pes; i++){
            for(int j=i+1; j<num_pes; j++){

                int new_cost = _swap_two_nodes(solution, current_cost, i, j);
                
                if(new_cost > G.number_of_edges()){
                    cerr << "erro ls\n";
                    exit(1);
                }

                if(new_cost > current_cost){
                    current_cost = new_cost;
                    is_improving = true;
                    stack.clear();
                }
                else{
                    int attempt_improve_cost = _swap_two_nodes(solution, new_cost, i, j);
                    if(current_cost < attempt_improve_cost){
                        current_cost = attempt_improve_cost;
                    } else{
                        _undo_swap();
                    }

                }
            }
        }
    }

    return current_cost;
}

double Solution::acceptance_probability(int deltaC, double temp) const{
    return exp(deltaC/temp);
}

int Solution::annealing(vector<int>&solution, int cost, const vector<double>&random_values, const vector<int>&random_nodes){

    stack.clear();

    double temp = MAX_TEMPERATURE;
    int current_cost = cost;
    int node_i, node_j;
    int k=0, i=1;

    while(temp>=MIN_TEMPERATURE && current_cost!=G.number_of_edges()){
        

        node_i = random_nodes[i-1];
        node_j = random_nodes[i];
        i+=2;

        int new_cost = _swap_two_nodes(solution, current_cost, node_i, node_j);
        int deltaC = new_cost - current_cost;

        if(new_cost > G.number_of_edges() || new_cost < 0){
            cerr << "erro annealing\n";
            exit(1);
        }

        if(deltaC>0 || random_values[k++] < acceptance_probability(deltaC, temp)){
            current_cost = new_cost;
            stack.clear();
        } else{
            int attempt_improve_cost = _swap_two_nodes(solution, new_cost, node_i, node_j);
            if(current_cost < attempt_improve_cost){
                current_cost = attempt_improve_cost;
            } else{
                _undo_swap();
            }
        }

        temp *= DECAY;
    }

    // delete[] nodes_to_swap;

    return current_cost;
}