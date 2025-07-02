#!/bin/bash
# Simple Docker-based script to run GAP benchmark with next_line prefetcher
echo "=== Downloading GAP trace file ==="
mkdir -p traces/gap
if [ ! -f traces/gap/bfs-10.trace.gz ]; then
    echo "GAP TRACE DNE!"
    exit 1
else
    echo "GAP trace file already exists, continuing..."
fi

echo "=== Building Berti simulator with Docker ==="
docker run -it -v$(pwd):/mnt --rm gcc:7.5.0 /bin/bash -c "cd mnt/ChampSim/Berti; ./build_champsim.sh hashed_perceptron no no no no no no no lru lru lru srrip drrip lru lru lru 1 no"

echo "=== Creating output directory ==="
mkdir -p output

echo "=== Running benchmark with Docker ==="
docker run -it -v$(pwd):/mnt --rm gcc:7.5.0 /bin/bash -c "cd mnt; ./ChampSim/Berti/bin/hashed_perceptron-no-no-no-no-no-no-no-lru-lru-lru-srrip-drrip-lru-lru-lru-1core-no -warmup_instructions 50000000 -simulation_instructions 200000000 -traces traces/gap/bfs-10.trace.gz> output/result.out"

echo "=== Done! ==="
echo "Results saved to: output/result.out"