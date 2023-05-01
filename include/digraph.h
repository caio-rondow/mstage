#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <set>
using namespace std;

class Digraph{

private:
    /* find neighbors/predecessors of a node */
    vector<vector<int>> neighbors_; 
    vector<vector<int>> predecessors_;
    /* parameters of the graph */
    int numEdges_, numNodes_;
    /* map the in/out degree of a node */
    int *out_degree_;
    int *in_degree_;

    /* maintain the object in standard order */
    void create();
    void destroy();

    /* traverse a graph */
    void dfs(int source, int current, vector<pair<int,int>>&edges, vector<bool>&visited) const;
    void bfs(int start, vector<bool>&visited, set<pair<int,int>>&visited_edges, vector<pair<int,int>>&edges) const;

public:

    // Construtors
    Digraph(const vector<vector<int>>&neighbors, 
            const vector<vector<int>>&predecessors, 
            int *out_degree, int *in_degree, int numEdges, 
            int numNodes);
    Digraph();

    // Copy constructor
    Digraph(const Digraph&);
    Digraph &operator=(const Digraph&);

    // Destructors 
    ~Digraph();

    // Methods
    /* to traverse */
    const vector<vector<int>> &neighbors() const;
    const vector<int> &neighbors(int node) const;
    const vector<vector<int>> &predecessors() const;
    const vector<int> &predecessors(int node) const;

    /* get the degree of a node */
    int out_degree(int node) const;
    int in_degree(int node) const;

    /* get the number of nodes/edges in the graph */
    int number_of_nodes() const;
    int number_of_edges() const;

    /* get a list of nodes/edges of the graph */ 
    vector<int> nodes() const;
    vector<pair<int,int>> edges() const;
    vector<pair<int,int>> bfs_edges() const;
    vector<pair<int,int>> dfs_edges() const;

    /* check if a node is multicast */
    bool is_multicast(int node) const;
};

#endif