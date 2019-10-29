make clean

echo -e "\nbenchmarking naive version"
make cell_distances_naive
echo -e "hyperfine --warmup 5 ./cell_distances_naive"
hyperfine --warmup 5 ./cell_distances_naive
./cell_distances_naive
# perf record --call-graph dwarf "./cell_distances_naive"
# cp perf.data ../FlameGraph/perf.data
# pushd ../FlameGraph
# perf script | ./stackcollapse-perf.pl |./flamegraph.pl > naive.svg
# popd


# echo -e "\nbenchmarking version 1"
# make cell_distances_ver1
# echo -e "hyperfine --warmup 5 \"./cell_distances_ver1\""
# hyperfine --warmup 5 ./cell_distances_ver1
# ./cell_distances_ver1

echo -e "\nbenchmarking version 2"
make cell_distances_ver2
echo -e "hyperfine --warmup 5 ./cell_distances_ver2"
hyperfine --warmup 5 ./cell_distances_ver2
./cell_distances_ver2
# perf record --call-graph dwarf "./cell_distances_ver2"
# cp perf.data ../FlameGraph/perf.data
# pushd ../FlameGraph
# perf script | ./stackcollapse-perf.pl |./flamegraph.pl > ver2.svg
# popd
