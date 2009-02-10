#!/bin/sh
# kernbench by Con Kolivas <kernbench@kolivas.org>
# based on a benchmark by Martin J. Bligh
#
# this bench (mbench) is modified by Roseman based on kernbench

trap 'echo "ABORTING";exit' 1 2 15

VERSION=0.2
# in version 0.1: statistic values are determined in mpirun, only elapsed-time is valuable (single value)
# in version 0.2: statistic values are determined in each processes created by mpirun, that's more accuracy (multi-value)

USERNAME=roseman

num_runs=2			# number of processes mbech-mpi
script_file=script.txt		# file containing action script of all processes
nloops=1			# number of loop calling mbench-mpi
logfile=mbench.log		# now, this value can't be modified. It's log file of each case (nice)
summary_logfile=summary.log	# summary file from many above logfile
niceval=0			# now, it's not used, may be deleted ...
batch=""			# batch="": do not run with batch policy

minnice=-20 			# min value of nice
maxnice=19			# max value of nice

go_flag=0	# if go_flag = 1: mbench-mpi may be called to run, else, it must wait until flag is turned on ... be deleted

#
# Parsing input
#
while getopts vn:l:f:r:p:m:M:hb i
do
	case $i in
		 h) echo "mbench is based on kernbench v$VERSION of Con Kolivas. It is modified by roseman"
				echo "Usage:"
				echo "mbench-v0.2.sh [-b] [-n runs] [-f script_file] [-l nloops] [-m min] [-M max] [-h] [-v]"
				echo "n : number of created process "
				echo "f : script file contains actions of all process"
				echo "l : nloops is a number of loops this mbench run"
				echo "m : min value of nice "
				echo "M : max value of nice "
				echo "h : print this help"
				echo "v : print version number"
				echo "b : run program with batch policy"
				exit ;;
		 v) echo "mbench is based on kernbench v$VERSION of Con Kolivas. It is modified by roseman" ; exit ;;
		 n) nruns=$OPTARG ;;
		 b) batch="-b";;
		 f) script_file=$OPTARG ;;
		 l) nloops=$OPTARG ;;
		 m) minnice=$OPTARG ;;
		 M) maxnice=$OPTARG ;;
	esac
done

lamhalt 
lamboot 
make
rm -f timelog

#
# Check needed tools
#
for i in time awk yes date
do
	iname=`which $i`
	if [[ ! -a $iname ]] ; then
		echo "$i not found in path, please install it; exiting"
		exit
	fi
done

time=`which time`

#
# Check input value
#
if [[ $nruns -gt 0 ]] ; then
	num_runs=$nruns
fi

if (($num_runs < 1)) ; then
	echo "No process is created. Nothing to do; exiting"
	exit
fi

if (($num_runs < 1)) ; then
	echo "Number of loops is zero or negative. Nothing to do; exiting"
	exit
fi

if (($minnice < -20)) ; then
	echo "Nice value must be in range from -20 to 19. Your minnice is modified to -20"
	minnice=-20
fi

if (($maxnice < -20)) ; then
	echo "Nice value must be in range from -20 to 19. Your maxnice is modified to -20"
	maxnice=-20
fi

if (($minnice > 19)) ; then
	echo "Nice value must be in range from -20 to 19. Your minnice is modified to 19"
	minnice=19
fi

if (($maxnice > 19)) ; then
	echo "Nice value must be in range from -20 to 19. Your maxnice is modified to 19"
	maxnice=19
fi

if (($maxnice < $minnice)) ; then
	echo "Are you crazy ?. Max-nice must be larger than or equal to min-nice. Game over"
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

#
# tab_string: make a string only contain /tab char. Number of tab is in $1
#
tab_string()
{
	tempstr=""
	for ((k=0; k<$1; k++))
	do
		tempstr="	"$tempstr
	done
	echo $tempstr
}
thisdate=`date +%Y%m%d_%H%M%S`
mkdir test_results > /dev/null 2>/dev/null
mkdir ./test_results/test$thisdate
summary_logfile="./test_results/test$thisdate/summary.log"
echo File script	:"$script_file" >> $summary_logfile
echo Loops		:"$nloops" >> $summary_logfile

echo "nice	Elapsed time	nice	User time	nice 	System time	nice	%CPU	nice	Context switch	nice	Sleeps" >> $summary_logfile


add_data_point()
{
    echo $@ | awk '{printf "%.6f %.6f %d", $1 + $2, $1 * $1 + $3, $4 + 1}'
}

#
# show_statistics : valueing average and deviation of data
# "avg" and "sdev" are output of show_statistics()
#
avg=0
sdev=0
show_statistics()
{
    avg=0
    sdev=0

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

	show_statistics $temp_elapsed
	elavg=$avg
	echo Elapsed Time "$avg ($sdev)" >> templog

	show_statistics $temp_user
	utavg=$avg
	echo User Time "$avg ($sdev)" >> templog
	
	show_statistics $temp_sys
	stavg=$avg
	echo System Time "$avg ($sdev)" >> templog
	
	show_statistics $temp_percent
	pcavg=$avg
	echo Percent CPU "$avg ($sdev)" >> templog

	show_statistics $temp_ctx
	csavg=$avg
	echo Context Switches "$avg ($sdev)" >> templog

	show_statistics $temp_sleeps
	slavg=$avg
	echo Sleeps "$avg ($sdev)" >> templog

	echo >> templog
	cat templog
	cat templog >> $logfile

	if (($i == $nloops)); then
		echo "$j	$elavg	$j	$utavg	$j	$stavg	$j	$pcavg	$j	$csavg	$j	$slavg " >> $summary_logfile
	fi
}

read_timelog()
{
	sum_eltime=0 
	sum_utime=0 
	sum_stime=0 
	sum_pcent=0
	sum_ctxswt=0
	sum_sleeptime=0
	while read eltime utime stime pcent ctxswt sleeptime;
	do
		sum_eltime=`echo $sum_eltime $eltime | awk '{print $1 + $2}'`
		sum_utime=`echo $sum_utime $utime | awk '{print $1 + $2}'`
		sum_stime=`echo $sum_stime $stime | awk '{print $1 + $2}'`
		sum_pcent=`echo $sum_pcent $pcent | awk '{print $1 + $2}'`
		sum_ctxswt=`echo $sum_ctxswt $ctxswt | awk '{print $1 + $2}'`
		sum_sleeptime=`echo $sum_sleeptime $sleeptime | awk '{print $1 + $2}'`
	done <timelog

	echo Loop $i-th >> ./test_results/test$thisdate/timelog_backup_$j
	cat timelog >> ./test_results/test$thisdate/timelog_backup_$j
	echo >> ./test_results/test$thisdate/timelog_backup_$j

	rm -f timelog
	echo $sum_eltime $sum_utime $sum_stime $sum_pcent $sum_ctxswt $sum_sleeptime > sum_timelog
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
	
#		strmpirun="mpirun -np $num_runs $time -a -f '"'%e %U %S %P %c %w'"' -o timelog ./mbench-mpi -n $num_runs -f $script_file -p $j"

		for (( i=1 ; i <= nloops ; i++ ))
		do
			echo $i-th loop of _nice_ $j...
			sleep 16
			mpirun -np $num_runs $time -a -f "%e %U %S %P %c %w" -o timelog ./mbench-mpi $batch -n $num_runs -f $script_file -p $j
			read_timelog
			read elapsed_time user_time sys_time percent ctx sleeps <sum_timelog
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

do_runs
