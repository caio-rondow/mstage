#include "../include/dataloader.h"

const Digraph DataLoader::read_dot(const string&filename, int copy) const{
    
    map<string,int> nodeMap;
    vector<vector<int>> neighbors, predecessors;
    map<int, int> count_in, count_out;
    int num_nodes=0, num_edges=0;

    // Define the regular expression pattern for matching an edge
    regex pattern("(\\w+)\\s*->\\s*(\\w+)");

    while(copy--){

        ifstream file(filename);
        if(!file.is_open()){
            cerr << "Error opening file " << filename << ".\n";
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            smatch match;
            if (regex_search(line, match, pattern)) {

                string from = match[1].str()+"_"+to_string(copy);
                string to   = match[2].str()+"_"+to_string(copy);


                // check if 'from_node' are mapped
                if (nodeMap.count(from) == 0) {
                    nodeMap[from] = num_nodes++;
                    count_out[nodeMap[from]]=0;
                    neighbors.push_back(vector<int>());
                    predecessors.push_back(vector<int>());

                    // cout << from << "\t" << nodeMap[from] << "\n"; 
                }

                // check if 'to_node' are mapped
                if (nodeMap.count(to) == 0) {
                    nodeMap[to] = num_nodes++;
                    count_in[nodeMap[to]]=0;
                    neighbors.push_back(vector<int>());
                    predecessors.push_back(vector<int>());

                    // cout << to << "\t" << nodeMap[to] << "\n"; 
                }

                // Add the edge to the graph
                int u = nodeMap[from];
                int v = nodeMap[to];
                neighbors[u].push_back(v);
                predecessors[v].push_back(u);
                count_out[u]++;
                count_in[v]++;
                num_edges++;
                // cout << to_string(nodeMap[from])+"->"+to_string(nodeMap[to])+"\n";
            }
        }
        file.close();
    }

    int *in_degree = new int[num_nodes];
    int *out_degree = new int[num_nodes];

    for(int i=0; i<num_nodes; i++){
        in_degree[i] = count_in[i];
        out_degree[i] = count_out[i];
    }

    Digraph G = Digraph(neighbors, predecessors, out_degree, in_degree, num_edges, num_nodes);

    delete[] in_degree;
    delete[] out_degree;

    return G;
}

const Architecture DataLoader::read_json(const string&filename) const{

    // Load the JSON file
    ifstream jsonFile(filename);

    if(!jsonFile.is_open()){
        cerr << "Could not open " + filename + ".\n";
        exit(1);
    }

    Json::Value root;
    jsonFile >> root;

    // Get the size of the input and output vectors
    int num_pes = root["num_pes"].asInt();

    // Read the input and output vectors for each PE
    vector<vector<int>> input(num_pes);
    vector<vector<int>> output(num_pes);
    
    for (int i = 0; i < num_pes; i++) {
        string peName  = "PE"+to_string(i);
        Json::Value pe = root[peName];

        int insize  = pe["input"].size();
        int outsize = pe["output"].size();
        
        for(int j=0; j<insize; j++){
            input[i].push_back( pe["input"][j].asInt() );
        }

        for(int j=0; j<outsize; j++){
            output[i].push_back( pe["output"][j].asInt() );
        }

    }

    Architecture arc(input,output,num_pes);
    jsonFile.close();
    
    return arc;
}