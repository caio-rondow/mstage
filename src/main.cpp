#include "../include/solution.h"
#include <chrono>
#define MAX_ITERATIONS 10

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
    cerr << "pre calculating random values...\n";
    /* INIT */
    string JsonFile  = argv[1];
    string DotFile   = argv[2];
    int extra        = atoi(argv[4]);
    string capacity  = argv[5];

    Solution SA(JsonFile, DotFile, 1, extra);

    Solution sols[10]={
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra),
        Solution(JsonFile, DotFile, 1, extra)
    };

    int optimum = SA.get_optimum();
    vector<vector<int>> InitialSolution(MAX_ITERATIONS, vector<int>(SA.size(), -1));
    vector<int> InitialCost(MAX_ITERATIONS);
    
    for(int i=0; i<MAX_ITERATIONS; i++){
        iota(InitialSolution[i].begin(), InitialSolution[i].end(), 0);
        random_shuffle(InitialSolution[i].begin(), InitialSolution[i].end());
        InitialCost[i] = sols[i].evaluate_initial_solution(InitialSolution[i]);
    }

    /* Pre-calculating random values */
    /* https://stackoverflow.com/questions/9878965/rand-between-0-and-1 */
    mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    uniform_real_distribution<double> unif(0, 1);
    uniform_int_distribution<int> iunif(0, SA.get_number_of_nodes()-1);

    int MAX_SWAPS = ((log10(MIN_TEMPERATURE) - log10(MAX_TEMPERATURE)) / log10(DECAY)) + 1;
    vector<vector<double>> random_values(MAX_ITERATIONS, vector<double>(MAX_SWAPS, 0));
    vector<vector<int>> random_nodes(MAX_ITERATIONS, vector<int>(2*MAX_SWAPS, 0));
    
    for(int i=0; i<MAX_ITERATIONS; i++){
        for(int j=0; j<MAX_SWAPS; j++){
            random_values[i][j] = unif(rng);
            random_nodes[i][j] = iunif(rng);
        }
    }

    cerr << "done!\n";
    cerr << "Starting SA\n";

    /* ANNEALING + LS */
    auto start = chrono::steady_clock::now();    
    int BestSA = -1;
    for(int i=0; i<MAX_ITERATIONS; i++){

        int CurrentCost = sols[i].annealing(InitialSolution[i], InitialCost[i], random_values[i], random_nodes[i]);
        CurrentCost = sols[i].local_search(InitialSolution[i], CurrentCost); 
        
        BestSA = max(CurrentCost, BestSA);
        if(BestSA == optimum)
            break;
    }
    auto end = chrono::steady_clock::now();
    auto sa_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();


    cout << DotFile << ", " << capacity << ", " << extra << ", " << BestSA << ", " << sa_elapsed << ", " << optimum << "\n";; 
















    
    // /*
    //     1. Guloso sequencial
    //     2. Guloso aleatório
    //     3. Guloso DFS
    //     4. Guloso BFS
    //     5. Busca Local no melhor
    //     6. Simulated Annealing + Busca Local
    // */

    // Solution random(json_file, dot_file, copies, extra);
    // Solution greedySeq(json_file, dot_file, copies, extra);
    // Solution greedyRnd(json_file, dot_file, copies, extra);
    // Solution greedyDFS(json_file, dot_file, copies, extra);
    // Solution greedyBFS(json_file, dot_file, copies, extra);
    // Solution simulated(json_file, dot_file, copies, extra);
    
    // int optimum = random.get_optimum();
    // int arcsize = random.size();

    // vector<int> randomSol(arcsize,-1);
    // vector<int> greedySeqSol(arcsize,-1);
    // vector<int> greedyRndSol(arcsize,-1);
    // vector<int> greedyDFSSol(arcsize,-1);
    // vector<int> greedyBFSSol(arcsize,-1);
    // vector<int> annealingSol(arcsize,-1);

    // iota(randomSol.begin(), randomSol.end(), 0);
    // random_shuffle(randomSol.begin(), randomSol.end());
    // iota(annealingSol.begin(), annealingSol.end(), 0);


    // /* SEQUENTIAL AND RANDOM */
    // auto start = chrono::steady_clock::now();
    // int rnd_cost = random.evaluate_initial_solution(randomSol);
    // auto end = chrono::steady_clock::now();
    // auto rnd_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();

    // /* GREEDY */
    // start = chrono::steady_clock::now();
    // int gseq_cost = greedySeq.greedy(greedySeqSol, "seq");
    // end = chrono::steady_clock::now();
    // auto gseq_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();
    
    // start = chrono::steady_clock::now();
    // int grnd_cost = greedyRnd.greedy(greedyRndSol, "rnd");
    // end = chrono::steady_clock::now();
    // auto grnd_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();

    // start = chrono::steady_clock::now();
    // int gdfs_cost = greedyDFS.greedy(greedyDFSSol, "dfs");
    // end = chrono::steady_clock::now();
    // auto gdfs_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();
    
    // start = chrono::steady_clock::now();    
    // int gbfs_cost = greedyBFS.greedy(greedyBFSSol, "bfs");
    // end = chrono::steady_clock::now();
    // auto gbfs_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();
    
    // /* GREEDY + LS */
    // int gls_cost=-1;
    // gls_cost = max(gls_cost, greedySeq.local_search(greedySeqSol,gseq_cost));
    // gls_cost = max(gls_cost, greedyRnd.local_search(greedyRndSol,grnd_cost));
    // gls_cost = max(gls_cost, greedyDFS.local_search(greedyDFSSol,gdfs_cost));

    // start = chrono::steady_clock::now();    
    // gls_cost = max(gls_cost, greedyBFS.local_search(greedyBFSSol,gbfs_cost));
    // end = chrono::steady_clock::now();
    // auto gls_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();

    // /* ANNEALING + LS */
    // start = chrono::steady_clock::now();    
    // int best_sa_cost = -1;
    // for(int i=0; i<MAX_ITERATIONS; i++){
    //     random_shuffle(annealingSol.begin(), annealingSol.end());
    //     int sa_cost = simulated.evaluate_initial_solution(annealingSol);

    //     sa_cost = simulated.annealing(annealingSol, sa_cost); // 6
    //     sa_cost = simulated.local_search(annealingSol, sa_cost); 
        
    //     best_sa_cost = max(sa_cost, best_sa_cost);
    //     if(best_sa_cost == optimum)
    //         break;
    // }
    // end = chrono::steady_clock::now();
    // auto sa_elapsed = chrono::duration_cast<chrono::seconds>(end - start).count();

    // cout << dot_file     << ",";
    // cout << capacity     << ",";
    // cout << extra        << ",";

    // // cout << rnd_elapsed  << ",";
    // // cout << gseq_elapsed << ",";
    // // cout << grnd_elapsed << ",";
    // // cout << gdfs_elapsed << ",";
    // // cout << gbfs_elapsed << ",";
    // // cout << gls_elapsed  << ",";
    // // cout << sa_elapsed   << ",";

    // cout << rnd_cost     << "(" << rnd_elapsed << ") ,";
    // cout << gseq_cost    << "(" << gseq_elapsed << ") ,";
    // cout << grnd_cost    << "(" << grnd_elapsed << ") ,";
    // cout << gdfs_cost    << "(" << gdfs_elapsed << ") ,";
    // cout << gbfs_cost    << "(" << gbfs_elapsed << ") ,";
    // cout << gls_cost     << "(" << gls_elapsed << ") ,";
    // cout << best_sa_cost << "(" << sa_elapsed << ") ,";
    // cout << optimum;
        
    return 0;
}
