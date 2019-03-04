## Compile

make all

########### CPU Benchmark Input ###########

<workload type (QP, HP, SP, DP)>
<number of threads (1,2,4)>


To compile the code, you can type bash commend as:

gcc MyCPUBench.c -pthread -lm -o cpu

And it would produce executing file--cpu.
Before running you can put the data your want to run in data folder, the default setting is data folder filling in all the data.
you can directly run this file by command: nohup ./cpu &
And running log would be in nohup.out and you can get result file naming as test.txt

or you can send this executing file to sbatch by running script:

sbatch ./run_script.sh

The script content is below:














