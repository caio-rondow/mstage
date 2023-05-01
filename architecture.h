#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <vector>
using namespace std;

class Architecture{

private:

    // PE representation
    vector<vector<int>> pe_input;
    vector<vector<int>> pe_output; 

public:
    // Constructor
    Architecture();

    // Copy Constructor
    Architecture(const Architecture&);
    Architecture &operator=(const Architecture&);

    // Methods
    const vector<int> &iport(int pe) const;
    const vector<int> &oport(int pe) const;
};

#endif