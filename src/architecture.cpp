#include "../include/architecture.h"

/* Constructor */
Architecture::Architecture(){}
Architecture::Architecture(const vector<vector<int>>&pe_input, const vector<vector<int>>&pe_output, int num_pes){
    _pe_output = pe_output;
    _pe_input  = pe_input;
    _num_pes   = num_pes;
};

/* Copy Constructor */
Architecture::Architecture(const Architecture&other){
    *this = other;
}

Architecture &Architecture::operator=(const Architecture&other){
    if (this == &other) return *this;
    _pe_input  = other._pe_input;
    _pe_output = other._pe_output;
    _num_pes   = other._num_pes;

    return *this;
}

/* Methods */
const vector<int> &Architecture::iport(int pe) const{ 
    
    return _pe_input[pe]; 
}

const vector<int> &Architecture::oport(int pe) const{ 
    return _pe_output[pe];
}

const int Architecture::size() const{ return _num_pes; }