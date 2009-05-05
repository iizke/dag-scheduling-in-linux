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
thisdate=`date +%Y%m%d_%H%M%S`
nnodes=9
loop=10
nice=19
density=20
./automateDag -n $nnodes -d $density -o ./tmp/dag$thisdate.txt
./dag2mbenchScript -f ./tmp/dag$thisdate.txt -o ./tmp/mb$thisdate.txt
../dag-sched/Release/dag-sched &
sleep 2
./mbench-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
sleep 4
./mbenchdag-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice 
killall dag-sched

sleep 2

thisdate=`date +%Y%m%d_%H%M%S`
./automateDag -n $nnodes -d $density -o ./tmp/dag$thisdate.txt
./dag2mbenchScript -f ./tmp/dag$thisdate.txt -o ./tmp/mb$thisdate.txt
../dag-sched/Release/dag-sched &
sleep 2
./mbench-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
sleep 4
./mbenchdag-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
killall dag-sched

sleep 2

thisdate=`date +%Y%m%d_%H%M%S`
./automateDag -n $nnodes -d $density -o ./tmp/dag$thisdate.txt
./dag2mbenchScript -f ./tmp/dag$thisdate.txt -o ./tmp/mb$thisdate.txt
../dag-sched/Release/dag-sched &
sleep 2
./mbench-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
sleep 4
./mbenchdag-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
killall dag-sched

sleep 2

thisdate=`date +%Y%m%d_%H%M%S`
./automateDag -n $nnodes -d $density -o ./tmp/dag$thisdate.txt
./dag2mbenchScript -f ./tmp/dag$thisdate.txt -o ./tmp/mb$thisdate.txt
../dag-sched/Release/dag-sched &
sleep 2
./mbench-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
sleep 4
./mbenchdag-v0.4.sh $batch -n $nnodes -l $loop -f ./tmp/mb$thisdate.txt -m $nice -M $nice
killall dag-sched
