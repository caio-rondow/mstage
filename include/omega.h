#ifndef OMEGA_H
#define OMEGA_H

#include "architecture.h"
#include "digraph.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

#define NUM_PES 256
#define FAIL -1
#define N 256
#define STAGE 4
#define EXTRA 1

using namespace std;

class Omega{

private:

    int **_switch;                          // shows route in switch
    int **_route_matrix;                    // shows if a wire is in use
    int _netsize, _st, _ex, _exsize, _mask; // parametros da rede
    

    // Aux. Functions
    void create();
    void destroy();
    
    int switch_code(int word, int col) const;
public:
    void desconcat(int word, int& input, int& extra, int& output) const;
    int concat(int input, int extra, int output) const;
    int window(int word, int col) const;
    // Constructor/Destructor
    Omega(int n=256, int st=4, int ex=1, int radix=4);
    ~Omega();

    // Methods
    int route(int input, int output);
    bool unroute(int word);
    void display() const;
    void display_switch() const;
    void clear();
    vector<vector<int>> get_net();
};

#endif
