#include "dataloader.h"

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
            cerr << "Error opening file " << filename << "\n";
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

    
}