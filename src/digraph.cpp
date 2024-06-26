#include "../include/digraph.h"
#include <iostream>
using namespace std;
Digraph::Digraph(){ create(); }

Digraph::~Digraph(){ destroy(); }

Digraph::Digraph(
        const vector<vector<int>>&neighbors, 
        const vector<vector<int>>&predecessors, 
        int *out_degree, int *in_degree, int numEdges, 
        int numNodes
    ):  neighbors_(neighbors), predecessors_(predecessors), 
        numEdges_(numEdges), numNodes_(numNodes){

    out_degree_ = vector<int>(numNodes);
    in_degree_  = vector<int>(numNodes);

    for(int i=0; i<numNodes; i++){
        out_degree_[i] = out_degree[i];
        in_degree_[i] = in_degree[i];
    }
}

void Digraph::create(){
    numEdges_ = numNodes_ = 0;
}

void Digraph::destroy(){
    numEdges_ = numNodes_ = 0;
}

Digraph::Digraph(const Digraph &other){
    // usando operador de atribuição 
    create();
    *this = other;
}

Digraph &Digraph::operator=(const Digraph &other){
    if(this==&other) return *this;
    destroy();
    numNodes_       = other.numNodes_;
    numEdges_       = other.numEdges_;
    neighbors_      = other.neighbors_;
    predecessors_   = other.predecessors_;
    out_degree_     = other.out_degree_;
    in_degree_      = other.in_degree_;


    return *this;
}

const vector<vector<int>> &Digraph::neighbors() const{ return neighbors_; }

const vector<int> &Digraph::neighbors(int node) const{ 
    if(node<0 || node >= number_of_nodes()){
        static const vector<int> empty;
        return empty;
    }
    return neighbors_[node]; 
}

const vector<vector<int>> &Digraph::predecessors() const{ return predecessors_; }

const vector<int> &Digraph::predecessors(int node) const{ 
    if(node<0 || node >= number_of_nodes()){
        static const vector<int> empty;
        return empty;
    }
    return predecessors_[node]; 
}

int Digraph::out_degree(int node) const{ return out_degree_[node]; }

int Digraph::in_degree(int node) const{ return in_degree_[node]; }

int Digraph::number_of_nodes() const{ return numNodes_; }

int Digraph::number_of_edges() const{ return numEdges_; }

vector<int> Digraph::nodes() const{
    vector<int> nodes(number_of_nodes());
    iota(nodes.begin(), nodes.end(), 0);
    return nodes;
}

vector<pair<int,int>> Digraph::edges() const{
    int parent=0, pos=0;
    vector<pair<int,int>> edges(number_of_edges());
    for(auto &node:neighbors()){
        for(auto neighbor:node){
            edges[pos] = make_pair(parent, neighbor);
            pos++;
        } parent++;
    }

    return edges;
}

void Digraph::bfs(int start, vector<bool>&visited, set<pair<int,int>>&visited_edges, vector<pair<int,int>>&edges) const{

    queue<int> q;
    q.push(start);
    visited[start]=true;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(auto v:neighbors(u)){
            pair<int,int> edge = make_pair(u,v);
            if(visited_edges.find(edge) == visited_edges.end()){
                visited_edges.insert(edge);
                edges.push_back(edge);
            }

            if(!visited[v]){
                q.push(v);
                visited[v]=true;
            }
        }
    }
}

vector<pair<int,int>> Digraph::bfs_edges() const{

    vector<bool> visited(number_of_nodes(), false);
    set<pair<int,int>> visited_edges;
    vector<pair<int,int>> edges;
    vector<int> nodes = this->nodes();

    sort(nodes.begin(), nodes.end(), [this](int a, int b){
        return in_degree(a) > in_degree(b);
    });
    
    for(auto &start:nodes){
        if(!visited[start]){
            bfs(start, visited, visited_edges, edges);
        }
    }

    return edges;
}

vector<pair<int,int>> Digraph::dfs_edges() const{

    /* start order */
    vector<int> nodes = this->nodes();

    vector<pair<int,int>> edges;
    vector<bool> visited(number_of_nodes(), false);

    for(auto &start:nodes){
        if(!visited[start])
            dfs(-1, start, edges, visited);
    }

    return edges;
}

void Digraph::dfs(int source, int current, vector<pair<int,int>>&edges, vector<bool>&visited) const{

    visited[current] = true;

    for(auto &predecessor:predecessors(current)){
        edges.push_back(make_pair(predecessor, current));
        if(!visited[predecessor])
            dfs(current, predecessor, edges, visited);
    }
}

bool Digraph::is_multicast(int node) const{
    return out_degree(node)>1;
}

void Digraph::add_edges_from(const vector<vector<int>>&neighbors, const vector<vector<int>>&predecessors){

    int k = numNodes_;
    for(auto &neighbor:neighbors){
        neighbors_.push_back(vector<int>());

        for(const int node:neighbor){
            cout << "eita\n";
            neighbors_[k++].push_back(node);
            numEdges_++;
        }
    }

    k = numNodes_;
    for(auto &predecessor:predecessors){
        predecessors_.push_back(vector<int>());

        for(const int &node:predecessor){
            cout << "eita\n";
            predecessors_[k++].push_back(node);
            numEdges_++;
        }
    }

    numNodes_ += neighbors.size();
}