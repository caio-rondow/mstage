#ifndef SOLUTION_H
#define SOLUTION_H

#include "omega.h"
#include "digraph.h"
#include "dataloader.h"
#include "architecture.h"

#define N 256
#define STAGE 4
#define EXTRA 0

class Solution {
private: 
    Omega net;
    Digraph G;
    Architecture arc;

    /* remove those functions later... (they are not part of this class) */
    bool _alloc_node_into_pe(vector<int>&node2pe,vector<int>&pe2node,int num_pes,int u, int v);
    int _eval(vector<int>&S);

public:
    // Constructor
    Solution(const string&json_file, const string&dot_file, int copy);

    // Methods 
    vector<int> greedy();
    vector<int> local_search(vector<int>&initial_solution);
    vector<int> annealing(vector<int>&initial_solution);
};

#endif