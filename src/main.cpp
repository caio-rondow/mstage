#include "../include/solution.h"

bool assert_ans(const vector<int>&ans, int n){
    for(int i=0; i<n; i++){

        if(ans[i]==-1) continue;

        for(int j=0; j<n; j++){
            
            if(ans[j]==-1) continue;

            if((i != j && ans[i] == ans[j])){
                cout << i << " " << j << "\n";
                cout << ans[i] << " " << ans[j] << "\n";
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char **argv){

    // int seed = 10;
    // srand(seed);

    /* INIT SOLUTION */
    string json_file = argv[1];
    string dot_file  = argv[2];
    int copy         = atoi(argv[3]);

    cout << dot_file << "\n";
    Solution solveby(json_file, dot_file, copy);
    vector<int> solution(solveby.size(),-1);

    solveby.info();

    /* GREEDY INITIAL SOLUTION */
    int greedy_cost = solveby.greedy(solution, "dfs");

    if(assert_ans(solution, solution.size())){
        cerr << "GM - invalid answer.\n";
        exit(1);
    }

    /* LOCAL SEARCH */
    int ls_cost = solveby.local_search(solution, greedy_cost);
    
    if(assert_ans(solution, solution.size())){
        cerr << "LS - invalid answer.\n";
        exit(1);
    }

    solveby.clear();

    /* METAHEURISTIC */
    int sa_cost;
    vector<int> sa_solution(solveby.size(),-1);
   
    iota(sa_solution.begin(), sa_solution.end(), 0);
   
    for(int i=0; i<1000; i++){

        random_shuffle(sa_solution.begin(), sa_solution.end());
        
        sa_cost = solveby.annealing( sa_solution, solveby.evaluate_initial_solution(sa_solution) );

        if(assert_ans(sa_solution, sa_solution.size())){
            cerr << "SA - invalid answer.\n";
            exit(1);
        }
        
        if(sa_cost == solveby.get_optimum()) 
            break;
    }

    /* RESULTS */ 
    cerr << "greedy     : " << greedy_cost << "\n";
    cerr << "greedy + LS: " << ls_cost << "\n";
    cerr << "annealing  : " << sa_cost << "\n";

    return 0;
}
