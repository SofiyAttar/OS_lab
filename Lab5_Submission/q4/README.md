# Lab 6 Question 4: Dining Philosophers — Deadlock Avoidance

### APPROACH: RESOURCE ORDERING

The solution uses **resource ordering** to prevent deadlock.

Each fork is implemented as a binary kernel semaphore using the custom `ksem` mechanism introduced earlier. Philosopher `i` requires forks `i` and `(i+1)%5`.

Before attempting to acquire the forks, each philosopher compares their numbers and always picks the **smaller-numbered fork first**. Thus, philosopher 4 acquires fork 0 before fork 4 instead of following the usual left-to-right order.

### DEADLOCK PREVENTION

The key condition for deadlock is a circular wait between processes.

With the ordering rule, a philosopher can hold a lower-numbered fork while waiting only for a higher-numbered fork. Therefore, along any chain of processes waiting for resources, the fork numbers must keep increasing.

Since the sequence cannot increase indefinitely and then return to the original fork, a circular wait cannot be formed. Hence, the resource-ordering rule eliminates deadlock.

### OTHER APPROACHES

Two other possible solutions were considered:

* **Limiting philosophers:** A semaphore could restrict the number of philosophers sitting at the table to four. This also prevents circular wait, but requires an additional semaphore and reduces possible concurrency.
* **Asymmetric fork selection:** Philosophers could use different acquisition orders depending on their number. This also breaks the circular-wait condition, but resource ordering provides a simpler and more uniform rule.

Resource ordering was selected because it requires minimal additional logic while allowing all philosophers to compete for the forks normally.

### PROGRESS / STARVATION

Each philosopher repeatedly alternates between thinking and eating, with bounded delays (`sleep(10)` while eating and `sleep(5)` while thinking). Both acquired forks are released after every eating phase.

The execution log shows that all **5 philosophers complete all 5 cycles** and the program terminates successfully. A deadlocked implementation would remain stuck instead of reaching termination.

### BUILD AND RUN

```bash
make clean && make qemu-nox
```

Inside xv6:

```text
dining
```
