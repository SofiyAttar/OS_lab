# Lab 6 Question 3: Readers-Writers — Fair Synchronization

### STRATEGY USED: FAIR READERS-WRITERS PROTOCOL

The implementation uses **3 reader processes and 2 writer processes**.

Shared state is maintained using `shm_get()`. The shared structure contains `read_count` and `shared_data`.

The synchronization protocol allows multiple readers to enter simultaneously, while writers require exclusive access to the shared resource.

### SEMAPHORES USED

Three semaphores coordinate the readers and writers:

```text
mutex   → protects read_count
wrt     → controls access to the shared resource
readTry → gate for incoming readers
```

The first reader acquires `wrt`, preventing writers from entering while readers are active. Additional readers can then enter without acquiring `wrt` individually.

When the last reader leaves, it releases `wrt`.

Writers acquire `wrt` directly, giving them exclusive access to the shared data.

### WRITER STARVATION PREVENTION

A reader-priority solution can allow a continuous stream of readers to keep a waiting writer out indefinitely.

To avoid this, `readTry` is used as a gate before readers enter the reader section. When a writer is waiting, new readers cannot continuously bypass it and must wait for the gate to become available.

This gives waiting writers an opportunity to acquire the shared resource between groups of readers.

### VERIFICATION

There are **3 readers and 2 writers**, with each process performing **3 iterations**.

The output demonstrates that:

* Multiple readers can access the data concurrently.
* A writer never overlaps with an active reader.
* Two writers do not execute the critical section simultaneously.
* Waiting writers are eventually able to proceed.

Each writer updates `shared_data` once per iteration. With 2 writers performing 3 updates each, the final value is expected to be:

```text
6
```

### BUILD AND RUN

```bash
make clean && make qemu-nox
```

Inside xv6:

```text
readwrite
```
