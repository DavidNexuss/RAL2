#!/bin/sh
function generate { 
  echo "==[GENERATE]============================="
  ./bin/main < $1 > $2
  echo "========================================="
  echo ""
}

mkdir -p results


# generate inputs/50_2_SinglePartial_0.txt results/50_2_SinglePartial_0.dat
# generate inputs/50_2_SinglePartial_1.txt results/50_2_SinglePartial_1.dat
# generate inputs/50_2_SinglePartial_2.txt results/50_2_SinglePartial_2.dat

# generate inputs/30_1_Single.txt results/30_1_Single.dat
# generate inputs/30_2_Single.txt results/30_2_Single.dat
# generate inputs/30_3_Single.txt results/30_3_Single.dat
# generate inputs/30_B_Single.txt results/30_B_Single.dat
# 
# generate inputs/50_1_Single.txt results/50_1_Single.dat
# generate inputs/50_2_Single.txt results/50_2_Single.dat
# generate inputs/50_3_Single.txt results/50_3_Single.dat
# generate inputs/50_B_Single.txt results/50_B_Single.dat
# 
# generate inputs/100_1_Single.txt results/100_1_Single.dat
# generate inputs/100_2_Single.txt results/100_2_Single.dat
# generate inputs/100_3_Single.txt results/100_3_Single.dat
# generate inputs/100_B_Single.txt results/100_B_Single.dat
# generate inputs/100_BB_Single.txt results/100_BB_Single.dat
# 
 generate inputs/50_1_Batch.txt results/50_1_Batch.dat
 generate inputs/50_2_Batch.txt results/50_2_Batch.dat
 generate inputs/50_3_Batch.txt results/50_3_Batch.dat
 generate inputs/50_B_Batch.txt results/50_B_Batch.dat
