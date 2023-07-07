# COMPILE CODE 
make -s

# CLEAN ANSWER 
> result.csv

# RUN CODE HERE
ARCH=misc/arch/default_arch128x2.json
for GRAPH in misc/merged_graph/*.dot; do
    ./build/apps/program $ARCH $GRAPH 1 >> result.csv
done 