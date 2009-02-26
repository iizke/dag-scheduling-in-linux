#mkdir ./test_results/`uname -r`
#test load : only cpubound
batch=""
#./mbench-v0.4.sh $batch -d n4 -n 4 -l 4 -f ./scripts/script_load.txt -m 0 -M 0
#./mbench-v0.4.sh $batch -d n5 -n 5 -l 4 -f ./scripts/script_load.txt -m 0 -M 0
#./mbench-v0.3.sh $batch -d n6 -n 6 -l 6 -f ./scripts/script_load.txt -m 19 -M 19
#./mbench-v0.3.sh $batch -d n7 -n 7 -l 6 -f ./scripts/script_load.txt -m 19 -M 19
#./mbench-v0.3.sh $batch -d n8 -n 8 -l 8 -f ./scripts/script_load.txt -m 0 -M 0

#test load + snd-rcv
#./mbench-v0.4.sh $batch -n 6 -l 4 -f ./scripts/script_load_sr.txt -m 0 -M 0
#./mbench-v0.3.sh -d otherbatch/cpuboud_sr -n 6 -l 8 -f ./scripts/script_load_sr1.txt -m 0 -M 0

#test load + some snd-rcv
#./mbench-v0.4.sh $batch -n 6 -l 4 -f ./scripts/script_load_ssr.txt -m 0 -M 0
#./mbench-v0.4.sh $batch -n 6 -l 4 -f ./scripts/script_load_ssr1.txt -m 0 -M 0

# test syncronous ssend

# create dag randomly
#mkdir tmp
./automateDag -n 4 -d 40 -o ./tmp/dag.txt
./dag2mbenchScript -f ./tmp/dag.txt -o ./tmp/mb.txt
../dag-sched/Debug/dag-sched &
sleep 1
./mbench-v0.4.sh $batch -n 5 -l 2 -f ./tmp/mb.txt -m 19 -M 19
sleep 4
./mbenchdag-v0.4.sh $batch -n 5 -l 2 -f ./tmp/mb.txt -m 19 -M 19 
killall dag-sched
