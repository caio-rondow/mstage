#include "architecture.h"

/* Constructor */
Architecture::Architecture(){};

/* Copy Constructor */
Architecture::Architecture(const Architecture&other){
    *this = other;
}

Architecture &Architecture::operator=(const Architecture&other){
    if (this == &other) return *this;
    pe_input  = other.pe_input;
    pe_output = other.pe_output;
}

/* Methods */
const vector<int> &Architecture::iport(int pe) const{ return pe_input[pe]; }

const vector<int> &Architecture::oport(int pe) const{ return pe_output[pe]; }