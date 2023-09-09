#include "../include/solution.h"
#include <chrono>
#define MAX_ITERATIONS 100

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
    int copies       = atoi(argv[3]);
    int extra        = atoi(argv[4]);
    string capacity  = argv[5];
    
    /*
        1. Guloso sequencial
        2. Guloso aleatório
        3. Guloso DFS
        4. Guloso BFS
        5. Busca Local no melhor
        6. Simulated Annealing + Busca Local
    */

    Solution random(json_file, dot_file, copies, extra);
    Solution greedySeq(json_file, dot_file, copies, extra);
    Solution greedyRnd(json_file, dot_file, copies, extra);
    Solution greedyDFS(json_file, dot_file, copies, extra);
    Solution greedyBFS(json_file, dot_file, copies, extra);
    Solution simulated(json_file, dot_file, copies, extra);
    
    int optimum = random.get_optimum();
    int arcsize = random.size();

    vector<int> randomSol(arcsize,-1);
    vector<int> greedySeqSol(arcsize,-1);
    vector<int> greedyRndSol(arcsize,-1);
    vector<int> greedyDFSSol(arcsize,-1);
    vector<int> greedyBFSSol(arcsize,-1);
    vector<int> annealingSol(arcsize,-1);

    iota(randomSol.begin(), randomSol.end(), 0);
    random_shuffle(randomSol.begin(), randomSol.end());
    iota(annealingSol.begin(), annealingSol.end(), 0);


    /* SEQUENTIAL AND RANDOM */
    auto start = chrono::steady_clock::now();
    int rnd_cost = random.evaluate_initial_solution(randomSol);
    auto end = chrono::steady_clock::now();
    auto rnd_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    /* GREEDY */
    start = chrono::steady_clock::now();
    int gseq_cost = greedySeq.greedy(greedySeqSol, "seq");
    end = chrono::steady_clock::now();
    auto gseq_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    
    start = chrono::steady_clock::now();
    int grnd_cost = greedyRnd.greedy(greedyRndSol, "rnd");
    end = chrono::steady_clock::now();
    auto grnd_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    start = chrono::steady_clock::now();
    int gdfs_cost = greedyDFS.greedy(greedyDFSSol, "dfs");
    end = chrono::steady_clock::now();
    auto gdfs_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    
    start = chrono::steady_clock::now();    
    int gbfs_cost = greedyBFS.greedy(greedyBFSSol, "bfs");
    end = chrono::steady_clock::now();
    auto gbfs_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    
    /* GREEDY + LS */
    int gls_cost=-1;
    gls_cost = max(gls_cost, greedySeq.local_search(greedySeqSol,gseq_cost));
    gls_cost = max(gls_cost, greedyRnd.local_search(greedyRndSol,grnd_cost));
    gls_cost = max(gls_cost, greedyDFS.local_search(greedyDFSSol,gdfs_cost));

    start = chrono::steady_clock::now();    
    gls_cost = max(gls_cost, greedyBFS.local_search(greedyBFSSol,gbfs_cost));
    end = chrono::steady_clock::now();
    auto gls_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    /* ANNEALING + LS */
    start = chrono::steady_clock::now();    
    int best_sa_cost = -1;
    for(int i=0; i<MAX_ITERATIONS; i++){
        random_shuffle(annealingSol.begin(), annealingSol.end());
        int sa_cost = simulated.evaluate_initial_solution(annealingSol);

        sa_cost = simulated.annealing(annealingSol, sa_cost); // 6
        sa_cost = simulated.local_search(annealingSol, sa_cost); 
        
        best_sa_cost = max(sa_cost, best_sa_cost);
        if(best_sa_cost == optimum)
            break;
    }
    end = chrono::steady_clock::now();
    auto sa_elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << dot_file     << ",";
    cout << capacity     << ",";
    cout << extra        << ",";

    // cout << rnd_elapsed  << ",";
    // cout << gseq_elapsed << ",";
    // cout << grnd_elapsed << ",";
    // cout << gdfs_elapsed << ",";
    // cout << gbfs_elapsed << ",";
    // cout << gls_elapsed  << ",";
    // cout << sa_elapsed   << ",";

    cout << rnd_cost     << ",";
    cout << gseq_cost    << ","; 
    cout << grnd_cost    << ",";
    cout << gdfs_cost    << ",";
    cout << gbfs_cost    << ",";
    cout << gls_cost     << ",";
    cout << best_sa_cost << ",";
    cout << optimum      << "\n";
        
    return 0;
}
