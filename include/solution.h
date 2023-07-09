#ifndef SOLUTION_H
#define SOLUTION_H

#include "omega.h"
#include "digraph.h"
#include "dataloader.h"
#include "architecture.h"
#include <random>
#include <chrono>

#define MAX_TEMPERATURE 1000.0
#define MIN_TEMPERATURE 0.0001
#define DECAY 0.99999

class Solution {
private: 
    Omega net;
    Digraph G;
    Architecture arc;

    int visited[N][N];

    int _swap_two_nodes(vector<int>&solution, int cost, int node_i, int node_j);
    int _remove_node_from_pe(const vector<int>&solution, int node, int pe);
    int _add_node_into_pe(const vector<int>&solution, int node, int pe);

    int _route(int peu, int pev);

    double acceptance_probability(int deltaC, double temp) const;

public:
    // Constructor
    Solution(const string&json_file, const string&dot_file, int copy, int extra);

    // Methods 
    int evaluate_initial_solution(const vector<int>&solution);
    int greedy(vector<int>&solution, const string&search="");
    int local_search(vector<int>&solution, int cost);
    int annealing(vector<int>&solution, int cost);

    int get_optimum() const;
    void info() const;
    int size() const;
    void clear();
};

#endif