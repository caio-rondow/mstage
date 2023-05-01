#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <vector>
#include <iostream>
using namespace std;

class Architecture{

private:

    // PE representation
    vector<vector<int>> _pe_input;
    vector<vector<int>> _pe_output;
    int _num_pes;

public:
    // Constructor
    Architecture();
    Architecture(const vector<vector<int>>&pe_input, const vector<vector<int>>&pe_output, int num_pes);

    // Copy Constructor
    Architecture(const Architecture&);
    Architecture &operator=(const Architecture&);

    // Methods
    const vector<int> &iport(int pe) const;
    const vector<int> &oport(int pe) const;
    const int size() const;
};

#endif