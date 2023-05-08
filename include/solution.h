#ifndef SOLUTION_H
#define SOLUTION_H

#include "omega.h"
#include "digraph.h"
#include "dataloader.h"
#include "architecture.h"

class Solution {
private: 
    Omega net;
    Digraph G;
    Architecture arc;

    bool _alloc_node_into_pe(vector<int>&node2pe,vector<int>&pe2node,int num_pes,int u, int v);

public:
    // Constructor
    Solution(const string&json_file, const string&dot_file, int copy);

    // Methods 
    vector<int> greedy();
    void local_search();
};

#endif