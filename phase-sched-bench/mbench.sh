#!/bin/sh
# kernbench by Con Kolivas <kernbench@kolivas.org>
# based on a benchmark by Martin J. Bligh
#
# this bench (mbench) is modified by Roseman based on kernbench

lamhalt 
lamboot 
make

trap 'echo "ABORTING";exit' 1 2 15

VERSION=0.1
USERNAME=roseman

num_runs=2		# number of processes mbech-mpi
script_file=script.txt
nloops=1		# number of loop calling mbench-mpi
logfile=mbench.log
summary_logfile=summary.log
niceval=0

minnice=-20 		# min value of nice
maxnice=19		# max value of nice

go_flag=0		# if go_flag = 1: mbench-mpi may be called to run, else, it must wait until flag is turned on

while getopts vn:l:f:r:p:m:M:h i
do
	case $i in
		 h) echo "mbench is based on kernbench v$VERSION of Con Kolivas. It is modified by roseman"
				echo "Usage:"
				echo "mbench.sh [-n runs] [-f script_file] [-l nloops] [-r logfile] [-m min] [-M max] [-h] [-v]"
				echo "n : number of created process "
				echo "f : script file contains actions of all process"
				echo "l : nloops is a number of loops this mbench run"
#				echo "p : nice value (priority) of process "
				echo "m : min value of nice "
				echo "M : max value of nice "
#				echo "r : logfile containning result of benchmark "
				echo "h : print this help"
				echo "v : print version number"
				exit ;;
		 v) echo "mbench is based on kernbench v$VERSION of Con Kolivas. It is modified by roseman" ; exit ;;
		 n) nruns=$OPTARG ;;
		 f) script_file=$OPTARG ;;
#		 p) niceval=$OPTARG ;;
		 l) nloops=$OPTARG ;;
#		 r) logfile=$OPTARG ;;
		 m) minnice=$OPTARG ;;
		 M) maxnice=$OPTARG ;;
	esac
done

for i in time awk yes date
do
	iname=`which $i`
	if [[ ! -a $iname ]] ; then
		echo "$i not found in path, please install it; exiting"
		exit
	fi
done

time=`which time`

if [[ $nruns -gt 0 ]] ; then
	num_runs=$nruns
fi

if (($num_runs < 1)) ; then
	echo "Nothing to do; exiting"
	exit
fi

#mem=`awk '/MemTotal/ {print $2}' /proc/meminfo`
#if [[ $mem -lt 2000000 && $max_runs -gt 0 ]] ; then
#	echo Less than 2Gb ram detected!
#	echo Maximal loads will not measure cpu throughput and may cause a swapstorm!
#	echo If you did not plan this, -M flag is recommended to bypass maximal load.
#fi

cpus=`grep -c processor /proc/cpuinfo`
echo
echo $cpus cpus found

echo Kernel `uname -r`
echo Performing $num_runs runs, looping $nloops times
echo

echo All data logged to $logfile

date >> $logfile
uname -r >> $logfile

add_data_point()
{
    echo $@ | awk '{printf "%.6f %.6f %d", $1 + $2, $1 * $1 + $3, $4 + 1}'
}

# "avg" and "sdev" are output of show_statistics()
avg=0
sdev=0
show_statistics()
{
#    avg=0
#   sdev=0

    case $3 in
	0)
	    echo "No data"
	    ;;
	1)
#	    echo $1
	    avg=$1
	    
	    ;;
	*)
	    avg=`echo $1 $3 | awk '{print $1 / $2}'`
	    var=`echo $1 $2 $3 | awk '{print ($2 - ($1 * $1) / $3) / ($3 - 1)}'`
	    sdev=`echo $var | awk '{print $1^0.5}'`
#	    echo "$avg ($sdev)"
	    ;;
    esac
}

do_log()
{
	echo "Average Run (std deviation):" > templog

#	echo Elapsed Time `show_statistics $temp_elapsed` >> templog
	show_statistics $temp_elapsed
	elavg=$avg
	echo Elapsed Time "$avg ($sdev)" >> templog

#	echo User Time  `show_statistics $temp_user` >> templog
	show_statistics $temp_user
	utavg=$avg
	echo User Time "$avg ($sdev)" >> templog
	
#	echo System Time  `show_statistics $temp_sys` >> templog
	show_statistics $temp_sys
	stavg=$avg
	echo System Time "$avg ($sdev)" >> templog
	
#	echo Percent CPU  `show_statistics $temp_percent` >> templog
	show_statistics $temp_percent
	pcavg=$avg
	echo Percent CPU "$avg ($sdev)" >> templog

#	echo Context Switches  `show_statistics $temp_ctx` >> templog
	show_statistics $temp_ctx
	csavg=$avg
	echo Context Switches "$avg ($sdev)" >> templog

#	echo Sleeps  `show_statistics $temp_sleeps` >> templog
	show_statistics $temp_sleeps
	slavg=$avg
	echo Sleeps "$avg ($sdev)" >> templog

	echo >> templog
	cat templog
	cat templog >> $logfile

	if (($i == $nloops)); then
		echo "$j	$elavg	$j	$stavg	$j	$csavg	$j	$slavg " >> $summary_logfile
	fi
}

do_runs()
{

#	echo $@
	for (( j=$minnice ; j <= $maxnice ; j++ ))	# nice value from -20 to 19
	do
		logfile="./test_results/test$thisdate/mbench_$j.log"
		echo All data of _nice_ $j logged to $logfile
		date >> $logfile
		uname -r >> $logfile

		temp_elapsed="a"
		temp_user=""
		temp_sys=""
		temp_percent=""
		temp_ctx=""
		temp_sleeps=""
		
		avg=0
		sdev=0

		strmpirun="mpirun -np $num_runs nice -n $j ./mbench-mpi -n $num_runs -f $script_file"
#		strmpirun="mpirun -np $num_runs $time -f '"'%e %U %S %P %c %w'"' -o timelog ./mbench-mpi -n $num_runs -f $script_file -p $j"
		for (( i=1 ; i <= nloops ; i++ ))
		do
			echo $i-th loop of _nice_ $j...
			sleep 8
			$strmpirun
#			$time -f "%e %U %S %P %c %w" -o timelog $strmpirun
#			$time -f "%e %U %S %P %c %w" -o timelog $stringrun > /dev/null 2>&1
			read elapsed_time user_time sys_time percent ctx sleeps <timelog
			echo cur_ctx = $ctx   cur_sleeps = $sleeps
			temp_elapsed=`add_data_point $elapsed_time $temp_elapsed`
			temp_user=`add_data_point $user_time $temp_user`
			temp_sys=`add_data_point $sys_time $temp_sys`
			temp_percent=`add_data_point $percent $temp_percent`
			temp_ctx=`add_data_point $ctx $temp_ctx`
			temp_sleeps=`add_data_point $sleeps $temp_sleeps`
			do_log
		done
	done
}

thisdate=`date +%Y%m%d_%H%M%S`
mkdir ./test_results/test$thisdate
summary_logfile="./test_results/test$thisdate/summary.log"
echo File script	:"$script_file" >> $summary_logfile
echo Loops		:"$nloops" >> $summary_logfile
echo "nice	Elapsed time	nice 	System time	nice	Context switch	nice	Sleeps " >> $summary_logfile

do_runs
