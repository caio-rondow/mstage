#ifndef OMEGA_H
#define OMEGA_H

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Omega{

private:

    int **_route_matrix;
    int **_row_status;
    int _netsize, _st, _ex, _exsize, _mask;  // parametros da rede
    vector<vector<int>> in_neighbor;
    vector<int> out_predecessor;

    // Aux; Functions
    int window(int word, int col) const;
    int switch_code(int word, int col) const;
    int concat(int input, int extra, int output) const;

public:
    // Constructor/Destructor
    Omega(int n=256, int st=4, int ex=1, int radix=4);
    ~Omega();

    // Methods
    bool route(int input, int output);
    void unroute(int word);
    void display() const;
    void clear();
};

#endif