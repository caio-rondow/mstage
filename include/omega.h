#ifndef OMEGA_H
#define OMEGA_H

#include "architecture.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

#define NUM_PES 128

using namespace std;

class Omega{

private:

    int **_switch;                          // shows route in switch
    int **_route_matrix;                    // shows if a wire is in use
    int _netsize, _st, _ex, _exsize, _mask; // parametros da rede
    
	vector<queue<int>> conn;    // keep track of the connections made in network

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
    bool route(int input, int output, int pein, int peout);
    int dealloc(const Architecture&arc, int pe);
    void display() const;
    void clear();
};

#endif
