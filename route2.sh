ARCH32="misc/arch/arch32.json"
ARCH64="misc/arch/arch64.json"
ARCH96="misc/arch/arch96.json"
ARCH128="misc/arch/arch128.json"

DIR="misc/instances"

# COMPILE CODE 
make -s

# CLEAN ANSWER 
echo "GRAPH,CAPACITY,EXTRA,GREEDY,SLS_GREEDY,ANNEALING,OPTIMUM" > result.csv

# RUN CODE
for GRAPH in "$DIR"/*; do
    for EXTRA in {0..4}; do
        ./build/apps/program $ARCH32 $GRAPH 2 $EXTRA 25   >> result.csv
        ./build/apps/program $ARCH64 $GRAPH 4 $EXTRA 50   >> result.csv
        ./build/apps/program $ARCH96 $GRAPH 6 $EXTRA 75   >> result.csv
        ./build/apps/program $ARCH128 $GRAPH 8 $EXTRA 100 >> result.csv
    done
    echo "$GRAPH done."
done