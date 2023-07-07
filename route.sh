arg=$1      # simulate a single graph
str="all"   # simulate all graphs

# .json ARCHS
ARCH=(
    # default_arch128x2.json  # Cplx8
    # default_arch128x2.json  # FilterRGB
    # default_arch128x2.json  # Fir16
    # default_arch128x2.json  # arf
    # default_arch128x2.json  # conv3
    # default_arch128x2.json  # cosine1
    # default_arch128x2.json  # cosine2
    # default_arch128x2.json  # ewf
    # default_arch128x2.json  # feedback_points
    # default_arch128x2.json  # fir
    # default_arch128x2.json  # fir1
    # default_arch128x2.json  # fir2
    # default_arch128x2.json  # h2v2_smooth
    # default_arch128x2.json  # horner_bezier_surf
    default_arch128x2.json  # interpolate_aux
    # jpeg_fdct_islow_config.json  # jpeg_fdct_islow
    # jpeg_idct_ifast_config.json  # jpeg_idct_ifast
    # k4n4op_config.json      # k4n4op
    # default_arch128x2.json  # mac
    # default_arch128x2.json  # motion_vectors
    # default_arch128x2.json  # mults1
    # default_arch128x2.json  # simple
)

# .dot GRAPHS
if [[ "$#" == 0 || "$arg" == "$str" ]]; then
    GRAPH=(
    # Cplx8.dot
    # FilterRGB.dot
    # Fir16.dot
    # arf.dot
    # conv3.dot
    # cosine1.dot
    # cosine2.dot
    # ewf.dot
    # feedback_points.dot
    # fir.dot
    # fir1.dot
    # fir2.dot
    # h2v2_smooth.dot
    # horner_bezier_surf.dot
    interpolate_aux.dot
    # jpeg_fdct_islow.dot
    # jpeg_idct_ifast.dot
    # k4n4op.dot
    # mac.dot
    # motion_vectors.dot
    # mults1.dot
    # simple.dot
    )
elif [ -e misc/graph/"$arg.dot" ]; then
    GRAPH=("$arg.dot")
fi

MAX_IN_NET=(
    # 2 # Cplx8
    # 2 # FilterRGB
    # 2 # Fir16
    # 4 # arf
    # 5 # conv3
    # 1 # cosine1
    # 1 # cosine2
    # 3 # ewf
    # 2 # feedback_points
    # 2 # fir
    # 2 # fir1
    # 3 # fir2
    # 2 # h2v2_smooth
    # 7 # horner_bezier_surf
    1 # interpolate_aux
    # 1 # jpeg_fdct_islow
    # 1 # jpeg_idct_ifast
    # 2 # k4n4op
    # 11 # mac
    # 4 # motion_vectors
    # 6 # mults1
    # 10 # simple
)

# COMPILE CODE 
make -s

# CLEAN ANSWER 
> result.csv

./build/apps/program misc/arch/default_arch128x2.json misc/merged_graph/ewf_ewf_ewf_conv3.dot 1 >> result.csv

# # RUN CODE HERE
# for ((i=0; i < ${#GRAPH[@]}; i++)) do
#     ./build/apps/program misc/arch/${ARCH[i]} misc/graph/${GRAPH[i]} ${MAX_IN_NET[i]} >> result.csv
# done