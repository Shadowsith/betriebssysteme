Named semaphores are stored in the file system.

During runtime of the example programms
  bin/barrierSyncAsym_A
  bin/barrierSyncAsym_B

you may list them via

$ ls -la /run/shm/
-rw-r--r--  1 regensbu regensbu       32 Jun 14 12:10 sem.DijkstraSem_SYNC


Experiment with options:

bin/barrierSyncAsym_A [nocreate]
bin/barrierSyncAsym_B [nocleanup]

Cleanup of global named semaphors is done via

./bin/barrierSyncCleanSem


