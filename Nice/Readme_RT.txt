DEMO in a virtual machine (don't use host system!)

If you use the host system, you have to run the demo (shell 1)
in a virtual console without X11.
For the second shell (root shell) you better login via ssh from remote.
Otherwise, the X11 display does not have enough prio to display anything.

Show that two SCHED_RR processes are strictly scheduled vi MLsndRR
Process with lesser prio has to wait for higher prio process

# Manual Page:
#
# man chrt

#----------
# Demo1

As a normal user

(bin/rt_worker1 0 &); (bin/rt_worker1 4 &)

In a second 'root' shell (you need CAP_SYS_NICE) change the
class and priorities
# Get pids via ps or obtain them from the output of the rt_workers
ps aux | grep rt_worker

# check prios
# chrt -p pid

chrt -p PID1
chrt -p PID2

# set prio of first process to SCHED_RR with RT prio -3
# chrt -p prio pid

chrt -r -p 3 PID1

NOTE: in case kill via 'pkill rt_worker'

# check prios either from output of workers or via commands
chrt -p PID1
chrt -p PID2

Observation:
Process with SCHED_OTHER prio is still scheduled once a while.
No starvation is taking place.

#----------
# Demo 2

# Start both processes again and give a real time prio to both of them

As a normal user

(bin/rt_worker1 0 &); (bin/rt_worker1 4 &)

In a second root shell (you need CAP_SYS_NICE) change class
and priorities.
# Get pids via ps or obtain them from the output of the rt_workers

ps aux | grep rt_worker

# Set prios of both processes in a single command (see below)
# The prio of first process to SCHED_RR is lesser than that of the snd one

chrt -r -p 2 PID1; chrt -r -p 3 PID1

NOTE: in case kill via 'pkill rt_worker'

# check prios either from output of workers or via commands
chrt -p PID1
chrt -p PID2

Observe:
- Process with lesser realtime prio is starved out
- Normal user processes are nearly starved out, including our GUI

