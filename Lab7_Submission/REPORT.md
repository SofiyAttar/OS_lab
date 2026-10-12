
# Consolidated Report — Assignment 7
## xv6: Threads, Synchronization, Deadlock, and Scheduling

**Course:** MA3104 (Operating Systems)  
**Name:** Sofiya Attar  
**Roll No:** 2401MC24  
**Environment:** xv6-public running on QEMU, with kernel extensions and source-level comments documenting the modifications.

---

## 1. Q1 — Kernel-Supported Threads (`clone()` / `join()`)

### Design and Implementation

- Each thread is represented by a `struct proc` entry with `isthread = 1`. Threads share the creator's page directory (`pgdir`) rather than creating a separate address space using `copyuvm()`. Each thread maintains its own kernel stack allocated through `allocproc()`.
- The `clone(fcn, arg, stack)` system call initializes the thread's trap frame. The instruction pointer (`eip`) is set to `fcn`, and the stack pointer (`esp`) is configured using the user-provided stack. The argument and a dummy return address are placed on the stack following the expected cdecl calling convention.
- Open-file descriptors are duplicated using `filedup()` so that threads can use standard input/output and other inherited file descriptors.
- The `join()` system call collects terminated threads marked as `isthread`. It releases the thread's kernel stack without freeing the shared page directory.
- The `wait()` system call skips thread processes to prevent accidental deallocation of a shared address space.
- When a thread invokes `fork()`, the resulting child is created as a regular process with its own address space using `copyuvm()`.
- The `struct proc` structure includes the `isthread` field. The system call numbers are `clone = 22` and `join = 23`.

### Results

Three threads were created to increment a shared counter. Observing a positive counter value in the parent demonstrates that the threads can access shared memory.

Without synchronization, concurrent updates can result in lost increments and an unreliable final value. However, a serialized single-CPU execution may occasionally produce `300000`; this does not guarantee race-free behavior.

---

## 2. Q2 — Counting Semaphores and Synchronization

### Design and Implementation

A fixed-size kernel array, `sems[10]`, stores semaphore instances defined using the following structure:

```c
struct semaphore {
    int value;
    struct spinlock lk;
    int inuse;
};
```

- **`sem_wait()`:** Acquires the semaphore lock and sleeps using `sleep(&sems[id], &lk)` while the semaphore value is non-positive. Once a resource becomes available, the value is decremented.
- **`sem_signal()`:** Increments the semaphore value and calls `wakeup()` to notify waiting processes.
- The waiting condition is checked inside a `while` loop to handle wakeups safely and ensure that a process rechecks the semaphore value before proceeding.
- The system calls are assigned the following numbers: `sem_init = 24`, `sem_wait = 25`, and `sem_signal = 26`.

### Counter Experiment: Before and After Synchronization

The same workload of three threads, each performing 100,000 increments, was used to evaluate synchronization.

| Version | Final Counter | Observation |
|---|---|---|
| Without synchronization | Potentially less than 300000 | Concurrent updates may be lost due to race conditions |
| With semaphore protection | Exactly 300000 in every recorded run | Mutual exclusion preserves the expected counter value |

### Producer-Consumer Experiment

A bounded buffer with a capacity of five items was used to test synchronization between a producer and a consumer.

The initial semaphore values were:

- `EMPTY = 5`: Number of available buffer slots.
- `FULL = 0`: Number of occupied buffer slots.
- `BUF = 1`: Mutual-exclusion semaphore protecting the buffer.

The producer generated 20 items, while the consumer removed them from the buffer.

The execution log recorded each production and consumption operation, including the corresponding buffer slot. When all five slots were occupied, the producer waited until space became available. Similarly, the consumer waited whenever the buffer was empty.

The experiment demonstrated FIFO processing of items 1 through 20, with every item consumed exactly once and no buffer overflow or underflow.

---

## 3. Q3 — Deadlock Detection and Avoidance

### Deadlock Detection Algorithm

The `detect_deadlock()` function implements a work-vector-based detection algorithm.

1. Initialize the work vector using the currently available resources.
2. Find an unfinished process whose outstanding request can be satisfied by the resources represented in the work vector.
3. Mark that process as finished and return its allocated resources to the work vector.
4. Repeat the process until no additional process can finish.
5. Any processes that remain unfinished are identified as belonging to the detected deadlocked set, under the assumptions of the algorithm.

### Test Cases and Results

| Test | Initial State | Result |
|---|---|---|
| T1 — Safe State | Available = (1,1); Allocations = (0,1), (1,0), (0,0); Requests = (1,0), (0,1), (1,1) | All processes can finish; no deadlock detected |
| T2 — Deadlocked State | Available = (0,0); Allocations = (1,0), (0,1); Requests = (0,1), (1,0) | Neither process can proceed; deadlocked set = {P0, P1} |
| T3 — Banker's Algorithm | Available = (1,1); Allocations = (1,1), (1,1); Maximum demands = (3,3), (2,2) | Initial state is safe; P1's request (1,1) is grantable, while P0's request (1,1) is denied because it leads to an unsafe state |

### Demonstration of a Real Pipe Deadlock

A separate experiment used two forked processes, A and B, to demonstrate circular waiting through pipes.

- Process A retained the write end of Pipe 1 and blocked while reading from Pipe 2.
- Process B retained the write end of Pipe 2 and blocked while reading from Pipe 1.
- Each process waited for an operation that could not complete while the other process remained blocked, creating a circular wait.

The resource relationships were represented using the deadlock detection logic, which identified the cycle A → B → A.

The deadlock was subsequently broken using `kill()` followed by `wait()`. The modified `piperead()` implementation checks whether a process has been killed and returns `-1` when appropriate, allowing the affected children to terminate.

---

## 4. Q4 — Priority-Based Preemptive Scheduling with Aging

### Design and Implementation

The default round-robin scheduler was modified to select runnable processes according to priority, while incorporating aging to reduce starvation.

- The `struct proc` structure includes a `priority` field and a `waitticks` counter.
- Priority values range from 1 to 20, where 1 represents the highest priority and 20 the lowest. The default priority is 10.
- The `set_priority()` and `get_priority()` system calls allow user programs to modify and retrieve process priorities.
- The scheduler selects the runnable process with the lowest numerical priority value.
- A rotating cursor, `rr_idx`, is used to distribute CPU time among processes with equal priorities.
- The selected process's waiting-time counter is reset when it is dispatched.
- The `age_procs()` function executes on timer ticks, increasing the waiting-time counters of runnable processes. When a process waits beyond 30 ticks, its numerical priority is reduced by one, improving its scheduling preference, and its waiting counter is reset.

### Observed Results

The `priotest` experiment was executed using a two-CPU QEMU configuration.

| Process | Priority | Approximate Completion Tick | Observation |
|---|---:|---:|---|
| proc0 | 1 | 100–110 | Receives high scheduling preference |
| proc1 | 5 | 100–110 | Receives high scheduling preference |
| proc2 | 10 | 200 | Completes after the higher-priority workload |
| proc3 | 20 | 300 | Priority improves from 20 to 19 and then 18 through aging |

### Completion-Time Comparison

The following representation summarizes the approximate completion times observed during the experiment.

```text
Priority 1  | #####...................  ~100 ticks
Priority 5  | #####...................  ~110 ticks
Priority 10 | ##########..............  ~200 ticks
Priority 20 | ###############.........  ~300 ticks
```

The results indicate a relationship between process priority and scheduling behavior. The lowest-priority process also completed, demonstrating how aging can improve its opportunity to execute and help prevent indefinite starvation.

---

## 5. Problems Faced and Resolutions

### (a) Thread Output Was Missing

Initially, threads did not produce the expected output. The `clone()` implementation had not duplicated the open-file table, leaving file descriptor 1 unavailable to the threads.

**Resolution:** Added a `filedup()` loop in `clone()`, following the approach used by `fork()`, so that threads could access inherited file descriptors.

### (b) Semaphore Wakeup Handling

The behavior of xv6's `wakeup()` required careful handling because it can wake multiple waiting processes. A naive implementation that decremented the semaphore and slept based on a negative value could result in incorrect synchronization.

**Resolution:** Used a `while (value <= 0)` condition with `sleep()` and a subsequent recheck, ensuring that a process proceeds only when the semaphore value permits it.

### (c) Race Conditions Were Not Always Visible

On a single CPU, short critical sections may execute in a serialized manner, allowing an unsynchronized counter to occasionally reach 300000 despite the presence of a race condition.

**Resolution:** Used concurrent execution and repeated testing to expose lost updates where possible. Semaphore protection ensured the expected result by serializing access to the shared counter.

### (d) Recovering from the Pipe Deadlock

The two processes involved in the circular wait could not make progress independently, so the deadlock did not resolve itself.

**Resolution:** Used `kill()` to terminate the blocked processes and `wait()` to collect their termination status. The `piperead()` implementation was modified to honor the killed-process flag and return `-1` where appropriate.

### (e) Starvation Under Strict Priority Scheduling

A scheduler that always selects the highest-priority runnable process can indefinitely postpone lower-priority processes under sustained high-priority workloads.

**Resolution:** Implemented a 30-tick aging threshold. Waiting processes receive gradual priority improvements, allowing lower-priority processes to obtain CPU time.

---

## 6. Conclusion

The four experiments demonstrate the implementation and evaluation of fundamental operating system mechanisms in xv6.

- **Threads:** Implemented `clone()` and `join()` to support threads sharing an address space while maintaining separate kernel stacks.
- **Synchronization:** Developed counting semaphores to protect shared data, eliminate lost counter updates, and coordinate producer-consumer operations.
- **Deadlock:** Implemented work-vector-based detection and Banker's algorithm for avoidance, validating the logic with three resource-allocation test cases and a pipe-based circular-wait demonstration.
- **Scheduling:** Extended the default scheduler with priority-based preemption and aging to improve scheduling preference while reducing starvation.

Together, these experiments provide practical insight into kernel modifications, concurrent execution, resource allocation, and CPU scheduling. The source code, kernel diffs, experiment-specific READMEs, and execution logs are organized under `q1/`, `q2/`, `q3/`, and `q4/`.
