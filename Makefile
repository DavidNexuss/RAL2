all: inputs/example.txt

inputs/example.txt: bin/main
	./bin/main -h > inputs/example.txt
bin/main: src/main.cpp src/config.hpp src/algorithm.hpp
	g++ src/main.cpp -O3 -march=native -mavx2 -o bin/main -fopenmp -I src
