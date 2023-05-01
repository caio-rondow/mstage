#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <map>
#include <regex>
#include <fstream>
#include <iostream>
#include <jsoncpp/json/json.h>

#include "digraph.h"
#include "architecture.h"

using namespace std;

class DataLoader{
public:
    const Digraph read_dot(const string&filename, int copy=1) const;
    const Architecture read_json(const string&filename) const;
};

#endif