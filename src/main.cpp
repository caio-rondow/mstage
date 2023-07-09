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

    srand(time(NULL));

    /* INIT SOLUTION */
    string json_file = argv[1];
    string dot_file  = argv[2];
    int copy         = atoi(argv[3]);
    int extra        = atoi(argv[4]);
    string capacity  = argv[5];

    cout << dot_file << "," << capacity << "," << extra << ",";
    Solution solveby(json_file, dot_file, copy, extra);
    vector<int> solution(solveby.size(),-1);

    // solveby.info();
    int optimum = solveby.get_optimum();

    /* GREEDY INITIAL SOLUTION */
    int greedy_cost = solveby.greedy(solution, "dfs");

    // if(assert_ans(solution, solution.size())){
    //     cerr << "GM - invalid answer.\n";
    //     exit(1);
    // }

    /* LOCAL SEARCH */
    int ls_cost = solveby.local_search(solution, greedy_cost);
    
    // if(assert_ans(solution, solution.size())){
    //     cerr << "LS - invalid answer.\n";
    //     exit(1);
    // }

    solveby.clear();

    /* ANNEALING */
    int sa_cost;
    int best_sa_cost=-1;
    vector<int> sa_solution(solveby.size(),-1);
   
    iota(sa_solution.begin(), sa_solution.end(), 0);
    random_shuffle(sa_solution.begin(), sa_solution.end());
    sa_cost = solveby.evaluate_initial_solution(sa_solution);

    for(int i=0; i<100000; i++){

        sa_cost = solveby.annealing( sa_solution,  sa_cost);

        // if(assert_ans(sa_solution, sa_solution.size())){
        //     cerr << "SA - invalid answer.\n";
        //     exit(1);
        // }

        if(sa_cost > best_sa_cost){
            best_sa_cost = sa_cost;
            // cerr << best_sa_cost << "\n"
        }
    }

    /* RESULTS */ 
    cout << greedy_cost << "," << ls_cost << "," << best_sa_cost << "," << optimum << "\n";

    return 0;
}
