# !/bin/sh
# kernbench by Con Kolivas <kernbench@kolivas.org>
# based on a benchmark by Martin J. Bligh
#
# this bench (mbench) is modified by Roseman based on kernbench
# 20061201

trap 'echo "ABORTING";exit' 1 2 15

# in version 0.1: statistic values are determined in mpirun, only elapsed-time is valuable (single value)
# in version 0.2: statistic values are determined in each processes created by mpirun, that's more accuracy (multi-value)
# in version 0.3: add more statistic values: true elapsed time and expected elapsed time + using tool schedtool
VERSION=0.4
USERNAME=roseman
num_runs=2			# number of processes mbech-mpi
script_file=script.txt		# file containing action script of all processes
nloops=1			# number of loop calling mbench-mpi
logfile=mbench.log		# now, this value can't be modified. It's log file of each case (nice)
summary_logfile=summary.log	# summary file from many above logfile
niceval=0			# now, it's not used, may be deleted ...
batch=""			# batch="": do not run with batch policy
subdir=""
minnice=-20 			# min value of nice
maxnice=19			# max value of nice
#
# Parsing input
#
while getopts vn:l:f:r:p:m:M:d:hb i
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
				echo "d : sub-directory "
				exit ;;
        v) echo "mbench is based on kernbench v$VERSION of Con Kolivas. It is modified by roseman" ; exit ;;
        n) nruns=$OPTARG ;;
        b) batch="-B";;
        f) script_file=$OPTARG ;;
        l) nloops=$OPTARG ;;
        m) minnice=$OPTARG ;;
        M) maxnice=$OPTARG ;;
        d) subdir=$OPTARG ;;
    esac
done

lamboot
make
rm -f timelog
#
# Check needed tools
#
for i in time awk yes date schedtool
do
    iname=`which $i`
    if [[ ! -a $iname ]] ; then
        echo "$i not found in path, please install it; exiting"
        exit
    fi
done
# get script file name in briefly
sf=$script_file
lsf=""

while [ "$sf" != "$lsf" ]
do 
    lsf=$sf
    sf=`echo $lsf | cut -f2- -d '/'`
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

thisdate=`date +%Y%m%d_%H%M%S`

mkdir ./test_results > /dev/null 2>/dev/null
mkdir ./test_results/_$lsf > /dev/null 2>/dev/null

if [ "$subdir" = "" ]; then
	subdir="./test_results/_$lsf"
else
	mkdir ./test_results/_$lsf/$subdir> /dev/null 2>/dev/null
	subdir="./test_results/_$lsf/$subdir"
fi

mkdir $subdir/avg > /dev/null 2>/dev/null
mkdir $subdir/sumary > /dev/null 2>/dev/null
mkdir $subdir/details > /dev/null 2>/dev/null

#mkdir $subdir/test$thisdate

summary_logfile="$subdir/sumary/`uname -r`.log"
echo `date` >> $summary_logfile

echo File script	:"$lsf" >> $summary_logfile
echo Loops		:"$nloops" >> $summary_logfile
if [ "$batch" = "-B" ]; then
	echo BATCH Scheduling >>$summary_logfile
else
	echo NORMAL Scheduling >>$summary_logfile
fi

echo "nice	Elapsed	BestET	Sum-ET	User	System	%CPU	Ctx-switch	Sleeps" >> $summary_logfile

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
	echo "Average-Run-(std- deviation):" > templog
	show_statistics $temp_true_elapsed
	true_elavg=$avg
	echo Elapsed-Time "$avg $sdev" >> templog
	show_statistics $temp_expected_elapsed
	expected_elavg=$avg
	echo Expected-Elapsed-Time "$avg $sdev" >> templog
	show_statistics $temp_sum_elapsed
	sum_elavg=$avg
	echo Sum-Elapsed-Times "$avg $sdev" >> templog
	show_statistics $temp_user
	utavg=$avg
	echo User-Time "$avg $sdev" >> templog
	show_statistics $temp_sys
	stavg=$avg
	echo System-Time "$avg $sdev" >> templog
	show_statistics $temp_percent
	pcavg=$avg
	echo Percent-CPU "$avg $sdev" >> templog
	show_statistics $temp_ctx
	csavg=$avg
	echo Context-Switches "$avg $sdev" >> templog
	show_statistics $temp_sleeps
	slavg=$avg
	echo Sleeps "$avg $sdev" >> templog
	echo >> templog
	cat templog
	cat templog >> $logfile
	if (($i == $nloops)); then
		echo "$j	$true_elavg	$expected_elavg	$sum_elavg	$utavg	$stavg	$pcavg	$csavg	$slavg " >> $summary_logfile
	fi
}

read_timelog()
{
	sum_eltime=0 
	true_eltime=0
	expected_eltime=0
	sum_utime=0 
	sum_stime=0 
	sum_pcent=0
	sum_ctxswt=0
	sum_sleeptime=0
	while read eltime utime stime pcent ctxswt sleeptime;
	do
		true_eltime=`echo $max_eltime $eltime | awk '{if ($2 > $1) {print $2} else {print $1} }'`
		sum_eltime=`echo $sum_eltime $eltime | awk '{print $1 + $2}'`
		sum_utime=`echo $sum_utime $utime | awk '{print $1 + $2}'`
		sum_stime=`echo $sum_stime $stime | awk '{print $1 + $2}'`
		sum_pcent=`echo $sum_pcent $pcent | awk '{print $1 + $2}'`
		sum_ctxswt=`echo $sum_ctxswt $ctxswt | awk '{print $1 + $2}'`
		sum_sleeptime=`echo $sum_sleeptime $sleeptime | awk '{print $1 + $2}'`
	done <timelog
	expected_eltime=`echo $sum_utime $sum_stime $cpus | awk '{print ($1+$2) / $3}'`
	echo Loop $i-th >> $subdir/details/nice$j'_'`uname -r`
	cat timelog >> $subdir/details/nice$j'_'`uname -r`
	echo >> $subdir/details/nice$j'_'`uname -r`
	rm -f timelog
	echo $true_eltime $expected_eltime $sum_eltime $sum_utime $sum_stime $sum_pcent $sum_ctxswt $sum_sleeptime > sum_timelog
}

do_runs()
{

#	echo $@
	for (( j=$minnice ; j <= $maxnice ; j++ ))	# nice value from -20 to 19
	do
		logfile="$subdir/avg/nice$j""_`uname -r`.log"
		echo All data of _nice_ $j logged to $logfile
		date >> $logfile
		uname -r >> $logfile
		temp_sum_elapsed="a"
		temp_true_elapsed="a"
		temp_expected_elapsed="a"
		temp_user=""
		temp_sys=""
		temp_percent=""
		temp_ctx=""
		temp_sleeps=""
		for (( i=1 ; i <= nloops ; i++ ))
		do
			echo $i-th loop of _nice_ $j...
			sleep 4
			mpirun -np $num_runs $time -a -f "%e %U %S %P %c %w" -o timelog schedtool -v $batch -n $j -e ./mbench-mpi -n $num_runs -f $script_file
#			mpirun -np $num_runs $time -a -f "%e %U %S %P %c %w" -o timelog schedtool -v -F -p 1 -e ./mbench-mpi -n $num_runs -f $script_file
#			mpirun -np $num_runs $time -a -f "%e %U %S %P %c %w" -o timelog ./mbench-mpi $batch -n $num_runs -f $script_file -p $j
			read_timelog
			read true_eltime expected_eltime sum_eltime user_time sys_time percent ctx sleeps <sum_timelog
			temp_sum_elapsed=`add_data_point $sum_eltime $temp_sum_elapsed`
			temp_true_elapsed=`add_data_point $true_eltime $temp_true_elapsed`
			temp_expected_elapsed=`add_data_point $expected_eltime $temp_expected_elapsed`
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
