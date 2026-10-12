
# 2401MC24_asg_07_MA3104 — OS Lab Assignment 7 (xv6)

> **Course:** MA3104 (Operating Systems)  
> **Name:** Sofiya Attar  
> **Roll No:** 2401MC24  
> **Platform:** xv6-public on QEMU with kernel extensions

## Overview

This assignment consists of four experiments implemented using the xv6 kernel, covering key operating system concepts: thread management, synchronization, deadlock handling, and CPU scheduling.

| # | Topic | Implementation |
|---|-------|----------------|
| Q1 | Threads | Added `clone()` and `join()` system calls to support kernel-managed threads sharing an address space |
| Q2 | Synchronization | Implemented counting semaphores (`sem_init/wait/signal`) to prevent race conditions and coordinate producer-consumer operations |
| Q3 | Deadlock | Implemented user-space deadlock detection using a work-vector algorithm, Banker’s algorithm for avoidance, and a pipe-based circular-wait demonstration |
| Q4 | Scheduling | Modified the default round-robin scheduler to support priority-based preemptive scheduling with aging |

## Repository Layout

```text
q1/   Thread management: threadtest.c, kernel diffs, README, and execution log
q2/   Semaphores: syntest.c, counter and producer-consumer tests, kernel diffs, and execution log
q3/   Deadlock handling: deadlock.c, three test matrices, pipe deadlock demonstration, kernel diffs, and execution log
q4/   Priority scheduling: priotest.c, trap/proc/scheduler diffs, and execution log
REPORT.md   Consolidated report covering design, results, and implementation challenges
```

## Quick Start

For each experiment, navigate to the corresponding xv6 source directory and refer to its README for specific instructions.

```bash
cd <question-folder>
make clean && make qemu-nox
```

Execute the appropriate user program from the xv6 shell:

```text
Q1: threadtest
Q2: syntest
Q3: deadlock
Q4: priotest
```

## Experimental Results

- **Q1 — Threads:** Three threads operated on a shared counter, demonstrating shared-address-space behavior. Unsynchronized concurrent updates resulted in unreliable counter values due to race conditions.

- **Q2 — Synchronization:** Semaphore-based synchronization consistently produced the exact counter value of **300000** across runs. The producer-consumer test also demonstrated correct buffer coordination without overflow or underflow.

- **Q3 — Deadlock:** The detection algorithm identified the deadlocked state involving P0 and P1 and recognized the safe test case. The Banker’s algorithm rejected an unsafe resource request. A pipe-based experiment demonstrated circular waiting between processes and how the deadlock could be broken.

- **Q4 — Scheduling:** Process completion behavior reflected the configured priorities of 1, 5, 10, and 20, with approximate completion times of 100, 110, 200, and 300 ticks. Aging improved the effective priority of a waiting process, illustrated by the progression from 20 to 19 and then to 18.
