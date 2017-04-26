08.04.2012: FABR

Play with a minimal unix shell

shell_01.c: C-Code for a minimal unix shell; no checks for errors (bad style)
shell_02.c: C-Code for a minimal unix shell; added error checking code (better)

Build:
$ make clean; make

mkdir bin
gcc -g -Wall shell_01.c -o bin/shell_01
gcc -g -Wall shell_02.c -o bin/shell_02

1) Check the PID of the shell you use for running the binary

$ echo $$
2374

Use this PID to select processes in the subsequent commands


2) Run the minimal shell without error checking code

bin/shell_01

In a second terminal print process status of all children of your
shell process in the first terminal window:

$ pstree -A -a -p 2374
bash,2374
  `-shell_01,2752

Type the following command into your minimal shell:

shell > ls
shell > bla

There is no output (neither normal nor error messages)
Reason: the commands ls and bla cannot be executed

Type in an existing command with its full path

shell > /bin/ls
bin  hello.sh  Makefile  shell_01.c  shell_02.c  sleeper.sh

Note: our minimal shell does not support arguments of commands.
Therefore, it is not possible to run a command like this:

shell > /bin/ls /etc

(no output)

shell > /usr/bin/printenv

MATE_DESKTOP_SESSION_ID=this-is-deprecated
SSH_AGENT_PID=2253
GPG_AGENT_INFO=/tmp/keyring-Yk6nYr/gpg:0:1
...
Kill the minimal shell by typing Ctrl-c to your minimal shell.

shell > ^C


3) Run the minimal shell with error checking code

bin/shell_02

In a second terminal print process status of all children of your
shell process in the first terminal window:

$ pstree -A -a -p 2374
bash,2374
  `-shell_02,3162

Type the following command into your minimal shell:

shell > ls
Warten auf Kind 3165
Fehler in execl: kann Kommando 
        ls
nicht ausfuehren
Rueckgabewert von Kind 3165 ist 1

shell > bla
Warten auf Kind 3167
Fehler in execl: kann Kommando 
        bla
nicht ausfuehren
Rueckgabewert von Kind 3167 ist 1

Now, there is output, namely error messages.
Reason: the commands ls and bla cannot be executed

Type in an existing command with its full path

shell > /bin/ls
Warten auf Kind 3169
bin  hello.sh  Makefile  Readme.txt  shell_01.c  shell_02.c  sleeper.sh
Rueckgabewert von Kind 3169 ist 0

Note: our minimal shell does not support arguments of commands.
Therefore, it is not possible to run a command like this:

shell > /bin/ls /etc

Warten auf Kind 3171
Fehler in execl: kann Kommando 
        /bin/ls /etc
nicht ausfuehren
Rueckgabewert von Kind 3171 ist 1

4) Run the script hello.sh from your minimal shell

shell > hello.sh
Warten auf Kind 3175
Hallo Du da
Rueckgabewert von Kind 3175 ist 42

Great! We are even able to start script files.

Reasons: 
First (a prerequisite)

$ ls -la hello.sh 
-rwxr-xr-x 1 regensbu regensbu 39 2012-04-07 19:01 hello.sh

The x-bits of the file hello.sh are set which marks the file
        hello.sh
to be an exetubale file.

Proof: remove 'x'-bit via
        chmod a-x hello.sh
and try to run the script once more

...

Add 'x'-bit again via
        chmod a+x hello.sh

Second, the execl system call passes the 'command' hello.sh to
the kernel. The kernel inspects the magic number at the start of
the file (here it is the ascii code of the shebang notation #!
which signals the kernel to start the interpreter command following
the shebang (here, it is /bin/bash)
The kernel code passes the sript file to the interpreter specified thus
invoking the execution of bin/bash an make it interpreting the
file hello.sh :-)

5) Run the script sleeper.sh from your minimal shell

hell > sleeper.sh
Warten auf Kind 3258
Schnarch fuer 10 Sekunden
Rueckgabewert von Kind 3258 ist 42

Run the command again, but while the sleeper is sleeping
inspect the children of you shell in the second window.

$ pstree -A -a -p 2374
bash,2374
  `-shell_02,3162
      `-sleeper.sh,3382 sleeper.sh
          `-sleep,3383 10

