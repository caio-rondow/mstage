#include "../include/solution.h"

bool assert_ans(const vector<int>&ans, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if((i != j && ans[i] == ans[j]) || ans[j] == -1){
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char **argv){

    /* INIT SOLUTION */
    string json_file = argv[1];
    string dot_file  = argv[2];
    int copy         = atoi(argv[3]);

    cout << dot_file << ", " << copy << ", ";
    
    Solution solveby(json_file, dot_file, copy);
    
    // /* GREEDY INITIAL SOLUTION */
    vector<int> ans = solveby.greedy();

    if(assert_ans(ans, ans.size())){
        cerr << "invalid answer.\n";
        exit(1);
    }

    /* LOCAL SEARCH */
    ans = solveby.local_search(ans);
    
    if(assert_ans(ans, ans.size())){
        cerr << "invalid answer.\n";
        exit(1);
    }

    // /* METAHEURISTIC */

    return 0;
}
