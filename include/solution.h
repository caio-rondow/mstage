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

public:
    // Constructor
    Solution(const string&json_file, const string&dot_file);

    // Methods 
    void greedy();
    void local_search();
};

#endif