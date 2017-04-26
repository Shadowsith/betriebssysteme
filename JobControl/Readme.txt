08.04.2012: FABR
Run several jobs in the background

Learn details about
- running processes from the shell
- job control and running commands in foreground or background
- the commands ps, pstree
- the tool htop
- setting up a pipeline of processes

#------------------------------------------------------------------
# Edit program worker.c and calibrate FACTOR for your machine
#------------------------------------------------------------------

The current setting is calibrated for the single core virtual machine debmed05
on notebook think.

Build the executable via

$ make clean; make

#------------------------------------------------------------------
# Run a worker and send it to the background using job control facilities
#------------------------------------------------------------------

Check the PID of the calling shell

$ echo $$
4038

Run worker and suspend the process via Ctrl-Z

$ bin/worker 50
PID=4115: 1
PID=4115: 2
PID=4115: 3
PID=4115: 4
PID=4115: 5
^Z
[1]+  Angehalten              bin/worker 50

Send the suspended process to the background:

regensbu@think:~/Bs/CodeExamples/JobControl
$ bg
[1]+ bin/worker 50 &
regensbu@think:~/Bs/CodeExamples/JobControl
$ PID=4115: 6
PID=4115: 7
PID=4115: 8
PID=4115: 9
...

Terminate process by typing Ctrl-C

^C
[1]+  Fertig                  bin/worker 50

#------------------------------------------------------------------
# Do the same again and inspect process tree
#------------------------------------------------------------------

$ bin/worker 50
PID=4291: 1
PID=4291: 2
PID=4291: 3
PID=4291: 4
PID=4291: 5
...

In the second terminal print the process tree of the shell
that startet the worker

$ pstree -A -a -p 4038
bash,4038
  `-worker,4291 50


Check available signals

$ kill -l  
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM
16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR
31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3
38) SIGRTMIN+4  39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12
53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7
58) SIGRTMAX-6  59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
63) SIGRTMAX-1  64) SIGRTMAX

Send the worker some signal for termination:
Possible signals for this purpose are e.g.

SIGHUP, SIGTERM, SIGKILL

kill -SIGTERM 4291

...
PID=4291:30 
PID=4291:31
Beendet

#------------------------------------------------------------------
# Set up a pipe
#------------------------------------------------------------------

$ echo $$
4038

$ bin/worker 10 | wc

The terminal is blocked until the pipe finishes its work.
In a second terminal check the process tree of the calling shell.

bash,4038
  |-wc,4417
  `-worker,4416 10


#------------------------------------------------------------------
# Set up a pipe and send it to the background
#------------------------------------------------------------------

$ bin/worker 30 | wc &
[1] 4427

This time, the terminal is not blocked.
Start two additional pipes

$ bin/worker 30 | wc &
[2] 4429
$ bin/worker 30 | wc &
[3] 4431

In the second terminal check the process tree

$ pstree -A -a -p 4038
bash,4038
  |-wc,4427
  |-wc,4429
  |-wc,4431
  |-worker,4426 30
  |-worker,4428 30
  `-worker,4430 30

#------------------------------------------------------------------
# Use command 'jobs' to show running jobs of the shell
#------------------------------------------------------------------

$ echo $$
4038

Run three jobs in the background.
Each one is a pipeline

bin/worker 50 | wc &
bin/worker 40 | wc &
bin/worker 30 | wc &

Check process tree of your shell in the second terminal

$ pstree -A -a -p 4038
bash,4038
  |-wc,4091
  |-wc,4093
  |-wc,4095
  |-worker,4090 50
  |-worker,4092 40
  `-worker,4094 30

Show running jobs in the first terminal window.
Must be the shell that controls the jobs

$ jobs
[1]   Läuft                  bin/worker 50 | wc &
[2]-  Läuft                  bin/worker 40 | wc &
[3]+  Läuft                  bin/worker 30 | wc &

After the individual jobs are finished the shell gives a notice

regensbu@think:~/Bs/CodeExamples/JobControl
$      30      60     381
[3]+  Fertig                  bin/worker 30 | wc
regensbu@think:~/Bs/CodeExamples/JobControl
$      40      80     511
[2]+  Fertig                  bin/worker 40 | wc
regensbu@think:~/Bs/CodeExamples/JobControl
$      50     100     641
[1]+  Fertig                  bin/worker 50 | wc

#------------------------------------------------------------------
# Use an even longer pipeline; enter awk
#------------------------------------------------------------------

$ echo $$
4038

$ bin/worker 4 | wc | awk '{printf "Es wurden %d Zeilen ausgegeben\n", $0;}'
Es wurden 4 Zeilen ausgegeben


#------------------------------------------------------------------
# Setup a pipeline and kill parts of the pipeline
#------------------------------------------------------------------

$ echo $$
4038

$ bin/worker 1000 | wc | awk '{printf "Es wurden %d Zeilen ausgegeben\n", $0;}'

Check the process tree in a second terminal

bash,4038
  |-awk,4537 {printf "Es wurden %d Zeilen ausgegeben\\n", $0;}
  |-wc,4536
  `-worker,4535 1000

Now, kill the worker process from the second terminal

$ kill -SIGTERM 4535

Es wurden 0 Zeilen ausgegeben

Reason: we killed the group leader. All processes in the
group were killed. Number of lines counted depends on
the whether the buffers inside the pipe were flushed in the meantime.

Do the same again and use tool htop with colums
    PID, PPID, PGRP
selected to see that the group leader of the pipe is the
first process in the pipe.

You may as well use the 'ps' command linke this:

$ ps -opid,ppid,pgrp,cmd --ppid 4038

  PID  PPID  PGRP CMD
 4535  4038  4535 bin/worker 1000
 4536  4038  4535 wc
 4537  4038  4535 awk {printf "Es wurden %d Zeilen ausgegeben\n", $0;}

