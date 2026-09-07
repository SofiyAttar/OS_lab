# Lab 6 Question 1: Peterson's Algorithm — Mutual Exclusion

### STRATEGY USED: PETERSON'S ALGORITHM

Peterson's algorithm is used to provide mutual exclusion between a parent and child process.

Since the processes created using `fork()` have separate address spaces, the synchronization variables cannot simply be ordinary global variables. A shared page is therefore obtained using the custom `shm_get()` system call after the fork.

The shared memory contains the `flag[2]`, `turn`, and `counter` variables. Each process sets its flag before entering the critical section and uses `turn` to give the other process priority when both processes are competing.

### WHY SHARED MEMORY IS REQUIRED

Peterson's algorithm depends on both processes seeing the same values of `flag[]` and `turn`.

If these variables were kept in normal process memory, the parent and child would operate on separate copies after `fork()`, making the synchronization ineffective.

`shm_get()` maps the same physical page into both processes, allowing the synchronization variables and counter to actually be shared.

### CRITICAL SECTION

The critical section performs a read-modify-write operation on the shared counter.

A delay is introduced between the read and write operations so that concurrent access would cause an incorrect result if mutual exclusion were not working.

Each process performs **10 critical-section iterations**.

### VERIFICATION

Both processes complete their iterations and the final counter value is:

```text
20
```

The expected value is obtained because Peterson's algorithm prevents the two processes from entering the critical section simultaneously.

No kernel semaphore or lock is used for the mutual exclusion in this question.

### BUILD AND RUN

```bash
make clean && make qemu-nox
```

Inside xv6:

```text
peterson
```
