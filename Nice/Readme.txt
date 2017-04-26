08.04.2012: FABR
Run several processes in parallel.

Learn details about
- running processes from the shell
- process priorities (nice values)
- the tool htop

0) It is best to run this demo in a virtual machine with a single core

1) edit program worker.c and calibrate FACTOR for your machine
The current setting is calibrated for the single core virtual machine debmed05
on notebook think.

Build the executable via

$ make clean; make

2) Run 2 worker processes in parallel

Run script run_test1.sh

./run_test1.sh

The script starts the workers in the background and then terminates.
Thus, the workers are orphaned and become children of the init process.

As an alternative to step 2 you may run the workers
directly from the shell like this:

Note: parantheses '(',')' ';' and '&' are essential!
a) syntactically, they are required due to & ;
b) the parenthesis start a sub-shell that exits after the launch
   of the worker. -> worker become orphaned zombies -> init adopts them

(bin/worker 0 &) ; (bin/worker 4 &)
...

                                PID=1477: 115
PID=1475: 116
                                PID=1477: 116
PID=1475: 117
                                PID=1477: 117
...

Check our hypotheses that the orphaned processes were adopted by init.
(Select process by PID)

$ ps -opid,ppid,cmd --pid 1475 1477
  PID  PPID CMD
 1475     1 bin/worker 0
 1477     1 bin/worker 4

Also check all processes of user (lars).

$ ps -u lars -opid,ppid,pgid,sid,cmd

What's the meaning of column SID ?

Also check all processes of user lars executing command worker
(select by command)

$ ps -opid,ppid,cmd -C worker
  PID  PPID CMD
 1475     1 bin/worker 0
 1477     1 bin/worker 4

3) In a second terminal window start htop and monitor process of 
   your user account (here it is lars)

$ htop -u lars
 or 
$ sudo htop -u lars
 or
$ su -
# htop -u lars

If you start htop as normal user you may only lower the priority of
your processes (give a positive nice value)
If you start it as user root you may even give the processes
a higher priority than normal (decrease nice values beyond normal level 0)

Use renice facility (+/-) to change priorities of the worker processes

Note: on a machine with more than one cpu (core) you first need
to pin all workers to the same core (set affinity).
This is done via the key 'a' inside htop. This feature is only available
on machines with more than one core.

Hint for htop:
- use F2 to reduce the number of columns displayed
  Remove: F9
  Escape Settings: (ESC and then up arrow; F10 is captured by mdm!)

- use F4 to filter the rows displayed (filter for process command)
  Filter: worker

4) Use 'pkill -f bin/worker' to kill all running worker processes

