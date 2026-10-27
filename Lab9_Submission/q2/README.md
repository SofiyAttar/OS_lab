# Q2 — Lazy Allocation (Demand Paging)

## Objective
Implement lazy allocation in xv6 by reserving virtual address space without immediately allocating physical memory. Allocate and map a page when the process first accesses it.

## Implementation
- Modified `sys_sbrk()` so positive growth reserves virtual address space without allocating physical pages.
- Added a page-fault counter to `struct proc` and initialized it for new processes.
- Added the `getfaults()` system call to report the current process's page-fault count.
- Added page-fault handling in `trap.c` to allocate, zero, and map pages on demand.
- Added the `alloclazy()` helper in `vm.c`.
- Updated `copyuvm()` to skip absent page mappings when copying a process.
- Added `lazytest.c` to test reservation, page faults, and invalid-address handling.

## Testing
Run the following command in the xv6 shell:

```text
lazytest
```

Observed results:
- Page faults after reserving 32 pages: `0`
- Page faults after touching five distinct pages: `5`
- The parent process survives the child's invalid memory access.

## Test Evidence
See `../Q2.png` for the successful test output.

## Source Changes
The `diffs/` directory contains the Q2-specific source patches.

