#!/usr/bin/env bash

make clean; make OPT_LEVEL=2 writing_to_hdd; ./writing_to_hdd;

make clean; make OPT_LEVEL=2 writing_to_ssd; ./writing_to_ssd;

start_time="$(date -u +%s.%N)"
for run in {1..10}
do
  cp -r /usr/include ~/usr_include_copy
done
end_time="$(date -u +%s.%N)"

elapsed="$(bc <<<"$end_time-$start_time")"
echo "Copying the folder /usr/include to hdd 10 times took $elapsed seconds"

start_time="$(date -u +%s.%N)"
for run in {1..10}
do
  cp -r /usr/include /run/mount/scratch/hpcuser004/usr_include_copy
done
end_time="$(date -u +%s.%N)"

elapsed="$(bc <<<"$end_time-$start_time")"
echo "Copying the folder /usr/include to ssd 10 times took $elapsed seconds"