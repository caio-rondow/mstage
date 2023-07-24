INSTANCES=14

GRAPH=(
'misc/graphs/Fir16.dot'
'misc/graphs/mults1.dot'
'misc/graphs/10horner_bezier_surf_1arf.dot'
'misc/graphs/simple.dot'
'misc/graphs/conv3.dot'
'misc/graphs/ewf.dot'
'misc/graphs/mac.dot'
'misc/graphs/Fir16_5arf.dot'
'misc/graphs/2ewf_2conv3_4horner_bezier_surf.dot'
'misc/graphs/sintetic.dot'
'misc/graphs/7sintetic_1mults1.dot'
'misc/graphs/5sintetic_1Fir16.dot'
'misc/graphs/sintetic.dot'
'misc/graphs/line.dot'
)

ARCH=(
'misc/archs_seq/arch_ex1_50.json'
'misc/archs_seq/arch_ex2_50.json'
'misc/archs_seq/arch_ex3_75.json'
'misc/archs_seq/arch_ex4_75.json'
'misc/archs_seq/arch_ex5_75.json'
'misc/archs_seq/arch_ex6_75.json'
'misc/archs_seq/arch_ex7_85.json'
'misc/archs_seq/arch_ex8_85.json'
'misc/archs_seq/arch_ex9_85.json'
'misc/archs_seq/arch_ex10_90.json'
'misc/archs_seq/arch_ex11_90.json'
'misc/archs_seq/arch_ex12_90.json'
'misc/archs_seq/arch_ex13_100.json'
'misc/archs_seq/arch_ex14_100.json'
)

COPY=(
2
6
1
14
7
4
16
1
1
7
1
1
8
1
)

CAPACITY=(
'49.21%'
'53.90%'
'74.21%'
'76.56%'
'73.82%'
'73.43%'
'81.25%'
'83.20%'
'82.81%'
'87.50%'
'92.96%'
'87.10%'
'100%'
'99.60%'
)

# COMPILE CODE 
make -s

# CLEAN ANSWER 
echo "GRAFO,CAPACIDADE,EXTRA,ALEATORIO,GULOSO_SEQ,GULOSO_RND,GULOSO_DFS,GULOSO_BFS,GULOSO_LS,ANNEALING,OPTIMUM" > result.csv

# RUN CODE
for ((i=0; i<$INSTANCES; i++)) do
    for EXTRA in {0..4}; do

        ./build/apps/program ${ARCH[i]} ${GRAPH[i]} ${COPY[i]} $EXTRA ${CAPACITY[i]} >> result.csv

    done
done