# Q1 — Page-Table Printing System Call

## Objective
Implement a `vmprint()` system call in xv6 to display valid user-page mappings and report the number of valid user pages and page-table pages.

## Implementation
- Added `vmprint()` in `vm.c` to traverse user virtual addresses below `KERNBASE`.
- Printed virtual addresses, physical addresses, and the Present (P), Writable (W), and User (U) permission bits.
- Counted valid user-page mappings and the page-table pages containing those mappings.
- Registered the `SYS_vmprint` system call in the syscall interface.
- Added `pttest.c` to test the page-table output before and after allocating four pages.

## Test
Run the following command in the xv6 shell:

```text
pttest
```

Expected observations:
- The page table is printed before and after allocation.
- The number of valid user pages increases by four.
- Heap growth is 16,384 bytes (4 × 4,096 bytes).

## Test Evidence
See `../Q1.png` for the successful test output.

## Source Changes
The `diffs/` directory contains the Q1 source patches.

