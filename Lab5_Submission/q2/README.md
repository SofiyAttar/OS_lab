# Lab 6 Question 2: Producer-Consumer — Bounded Buffer

### STRATEGY USED: COUNTING SEMAPHORES

The bounded-buffer problem is implemented using the custom kernel counting semaphores together with shared memory.

The test uses **one producer, one consumer, a buffer of size 5, and 20 total items**.

The producer and consumer share the buffer and its indices through the page obtained using `shm_get()`.

### SEMAPHORES USED

Three semaphores are used:

```text
empty = 5
full  = 0
mutex = 1
```

`empty` represents the number of free buffer positions, while `full` tracks the number of items available for consumption. `mutex` ensures that only one process modifies the buffer at a time.

The producer waits for an empty position before inserting an item. The consumer waits for an available item before removing one.

### BLOCKING BEHAVIOR

The producer is intentionally faster than the consumer.

The producer uses a delay of **2 ticks**, while the consumer waits for **20 ticks**. As a result, the producer can fill all five buffer positions and then block on the `empty` semaphore.

When the consumer removes an item, it signals `empty`, allowing the producer to continue.

This demonstrates that the semaphore implementation actually blocks and wakes processes instead of relying on busy waiting.

### VERIFICATION

The producer generates **20 items**, and the consumer removes them in order.

The execution confirms that:

* The buffer never exceeds its capacity.
* No item is lost.
* No item is consumed more than once.
* The producer correctly waits when the buffer is full.

### BUILD AND RUN

```bash
make clean && make qemu-nox
```

Inside xv6:

```text
prodcons
```
