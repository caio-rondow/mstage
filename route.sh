INSTANCES=12

GRAPH=(
    misc/graphs/2_Fir16.dot                             # a0
    misc/graphs/6_Mults1.dot                            # a5
    misc/graphs/4_Ewf.dot                               # a2
    misc/graphs/7_Conv3.dot                             # a7
    misc/graphs/16_Mac.dot                              # a8
    misc/graphs/2_Ewf_2_Conv3_4_Horner_Bezier_Surf.dot  # a6
    misc/graphs/1_Fir16_5_Arf.dot                       # a9
    misc/graphs/5_Synthetic_1_Fir16.dot                 # a4
    misc/graphs/7_Synthetic.dot                         # a0
    misc/graphs/7_Synthetic_1_Mults1.dot                # a3
    misc/graphs/1_Pipeline.dot                          # a1
    misc/graphs/8_Synthetic.dot                         # a0
)


ARCH=(
'misc/archs_seq/A0.json'
'misc/archs_seq/A5.json'
'misc/archs_seq/A2.json'
'misc/archs_seq/A7.json'
'misc/archs_seq/A8.json'
'misc/archs_seq/A6.json'
'misc/archs_seq/A9.json'
'misc/archs_seq/A4.json'
'misc/archs_seq/A0.json'
'misc/archs_seq/A3.json'
'misc/archs_seq/A1.json'
'misc/archs_seq/A0.json'
)

CAPACITY=(
    '49.21%' # 2 fir16
    '53.90%' # 6 mults1
    '73.43%' # 4 ewf
    '73.82%' # 7 conv3
    '81.25%' # 16 mac
    '82.81%' # 2 ewf 2 conv3 4 horner
    '83.20%' # 1 fir16 5 arf
    '87.10%' # 5 synthetic 1 fir
    '87.50%' # 7 synthetic
    '92.96%' # 7 synthetic 1 mults1
    '99.60%' # 1 Pipeline
    '100.00%' # 8 synthetic
)

COPY=1

# COMPILE CODE 
make -s

# CLEAN ANSWER 
# echo "GRAFO,CAPACIDADE,EXTRA,ALEATORIO,GULOSO_SEQ,GULOSO_RND,GULOSO_DFS,GULOSO_BFS,GULOSO_LS,ANNEALING,OPTIMUM" > result.csv
echo "GRAFO,CW,EXTRA,SA,TEMPO_SA_SEGUNDOS,OPTIMUM" > result.csv

# RUN CODE
for ((i=0; i<$INSTANCES; i++)) do
    for EXTRA in {0..4}; do

        ./build/apps/program ${ARCH[i]} ${GRAPH[i]} $COPY $EXTRA ${CAPACITY[i]} >> result.csv

    done
done