#ifndef OMEGA_H
#define OMEGA_H

#include "architecture.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Omega{

private:

    int **_route_matrix;                     // shows route in switch
    int **_row_status;                       // shows if a wire is in use
    int _netsize, _st, _ex, _exsize, _mask;  // parametros da rede
    vector<vector<int>> _in_neighbor;
    vector<int> _out_predecessor;

    // Aux. Functions
    void create();
    void destroy();
    
    int window(int word, int col) const;
    int switch_code(int word, int col) const;
    int concat(int input, int extra, int output) const;
    void unroute(int word);

public:
    // Constructor/Destructor
    Omega(int n=256, int st=4, int ex=1, int radix=4);
    ~Omega();

    // Methods
    bool route(int input, int output);
    void dealloc(const Architecture&arc, int pe);
    void display() const;
    void clear();
};

#endif