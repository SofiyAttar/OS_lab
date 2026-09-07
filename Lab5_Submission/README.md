# xv6 Process Synchronization Lab

This repository contains the implementation of **Operating Systems Lab Assignment 6**, covering four classical process synchronization problems in the **xv6** teaching operating system.

The xv6 kernel was extended with custom **shared memory** and **counting semaphore** support to allow processes to communicate and synchronize while solving the problems.

**Course:** MA3105 — Operating Systems
**Student:** Sofiya Attar
**Roll No.:** 2401MC24

---

## 📁 Contents

| Directory       | Problem              | Main Concept         |
| --------------- | -------------------- | -------------------- |
| `q1/`  | Peterson's Algorithm | Mutual Exclusion     |
| `q2/`  | Producer-Consumer    | Bounded Buffer       |
| `q3/` | Readers-Writers      | Fair Synchronization |
| `q4/`           | Dining Philosophers  | Deadlock Avoidance   |

Each question contains its own xv6 modifications, README, diffs, and execution output.

---

## 🔧 xv6 Extensions

### Shared Memory

A custom `shm_get()` system call was introduced to create a shared memory page between processes.

The page is allocated using `kalloc()` and mapped at:

```text
SHM_VA = 0x60000000
```

This allows processes to access common variables despite having separate address spaces. The virtual-memory cleanup code was also adjusted so that the shared page is not incorrectly released when a process exits.

### Counting Semaphores

A kernel-level counting semaphore was added using xv6's existing `spinlock`, `sleep()`, and `wakeup()` mechanisms.

The following system calls are available to user programs:

```text
sem_init(which, count)
sem_wait(which)
sem_signal(which)
```

These semaphores are used throughout the synchronization problems wherever blocking and resource coordination are required.

---

## 🧩 Problem Implementations

### 1. Peterson's Algorithm

Peterson's algorithm is used to provide mutual exclusion between a parent and child process.

The synchronization variables `flag[]` and `turn`, along with the shared counter, are placed in the shared memory page. Both processes repeatedly perform a delayed read-modify-write operation inside the critical section.

The final counter value is **20**, confirming that the critical section is accessed safely without using kernel locks for the mutual exclusion itself.

### 2. Producer-Consumer

A bounded buffer of **5 slots** is shared between one producer and one consumer, with **20 items** transferred in total.

Three semaphores coordinate the buffer:

```text
empty = 5
full  = 0
mutex = 1
```

The producer is made faster than the consumer so that the buffer eventually becomes full. The producer then blocks on `empty` and resumes when the consumer frees a slot.

All 20 items are transferred successfully without duplication or loss.

### 3. Readers-Writers

The implementation uses **3 readers and 2 writers** and follows a fair synchronization strategy.

A `readTry` semaphore acts as a gate for incoming readers. Once a writer is waiting, new readers cannot continuously enter ahead of it.

As a result, readers can access the shared resource concurrently, while writers receive exclusive access and are not indefinitely postponed.

### 4. Dining Philosophers

The Dining Philosophers problem is implemented with **5 philosophers**, where each fork is represented by a binary semaphore.

To avoid deadlock, the forks are acquired using **resource ordering**: every philosopher picks the lower-numbered fork first and the higher-numbered fork second.

This removes the possibility of a circular wait. For example, philosopher 4 picks fork 0 before fork 4 instead of following the usual left-then-right order.

Each philosopher performs **5 thinking/eating cycles**, and all five processes complete successfully.

---

## ▶️ Running the Programs

### Requirements

* Linux / WSL
* `gcc`
* `make`
* `qemu-system-i386`

Enter any question directory and run:

```bash
make clean
make qemu-nox
```

For example:

```bash
cd q4
make clean
make qemu-nox
```

Then, for Q4, run:

```text
dining
```

The corresponding question directories can be used similarly for the other implementations.
