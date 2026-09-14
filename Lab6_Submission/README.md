
**Roll Number:** 2401MC24
**Course:** Operating Systems Lab

This assignment implements four deadlock-handling and synchronization problems using xv6-public (x86).

### Questions

**Q1 - Banker's Algorithm:** Determines whether a system is in a safe state before granting resource requests, preventing deadlock through safe-state checking.

**Q2 - Deadlock Detection:** Uses a Resource Allocation Graph and cycle detection to identify whether processes are involved in a deadlock.

**Q3 - Deadlock Prevention:** Uses resource ordering to prevent circular wait. Processes acquire resources in a fixed order, ensuring deadlock cannot occur.

**Q4 - Synchronization and Deadlock Avoidance:** Demonstrates synchronization between processes accessing multiple resources while using a deadlock-avoidance strategy to ensure safe execution.

### Folder Structure

* `q1/` – Banker's Algorithm
* `q2/` – Deadlock Detection
* `q3/` – Resource Ordering
* `q4/` – Synchronization and Deadlock Avoidance
* `screenshots/` – Output logs
* `tests/` – Test programs
* `diffs/` – Source code changes

Each question uses a separate xv6 copy based on the clean xv6-public source.

### Running

```bash
cd q1
make clean
make
make qemu
```

Run the corresponding program inside xv6. The same steps apply to Q2–Q4.

All implementations, test files, diffs, and output logs are included in this submission.

