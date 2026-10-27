# Assignment 9 — xv6 Memory Management and Page Replacement

**Name:** Sofiya Attar  
**Roll Number:** 2401MC24

## Overview

This assignment implements and tests three components in xv6-public:

1. **Q1 — Page-Table Printing:** Adds a system call to print valid user-page mappings, permission bits, and page-table statistics.
2. **Q2 — Lazy Allocation:** Reserves virtual memory without immediate physical allocation and allocates pages on demand through page-fault handling.
3. **Q3 — Page Replacement Algorithms:** Simulates FIFO and LRU page replacement using a fixed reference string and different frame counts.

## Repository Structure

```text
assignment_9/
├── README.md
├── Q1.png
├── Q2.png
├── Q3.png
├── q1/
│   ├── README.md
│   ├── pttest.c
│   └── diffs/
├── q2/
│   ├── README.md
│   ├── lazytest.c
│   └── diffs/
└── q3/
    ├── README.md
    ├── paging_sim.c
    └── diffs/
```

## Q1 — Page-Table Printing

### Implementation

- Added the `vmprint()` system call.
- Printed virtual addresses, physical addresses, and P/W/U permission bits.
- Counted valid user pages and page-table pages.
- Added `pttest.c` to test page mappings before and after allocation.

### Test

Run in the xv6 shell:

```text
pttest
```

### Observed Results

- Valid user pages before allocation: 3
- Valid user pages after allocating four pages: 7
- Increase in mapped pages: 4
- Heap growth: 16,384 bytes

See [`Q1.png`](Q1.png).

## Q2 — Lazy Allocation

### Implementation

- Modified `sys_sbrk()` to reserve virtual address space for positive growth without immediately allocating physical pages.
- Added a per-process page-fault counter.
- Added the `getfaults()` system call.
- Implemented lazy page allocation and mapping in the page-fault handler.
- Updated `copyuvm()` to skip absent page mappings.
- Added `lazytest.c`.

### Test

Run in the xv6 shell:

```text
lazytest
```

### Observed Results

- Page faults after reserving 32 pages: 0
- Page faults after touching five distinct pages: 5
- Parent process survived the child's invalid memory access.

See [`Q2.png`](Q2.png).

## Q3 — Page Replacement Algorithms

### Reference String

`7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`

Total references: 20

### Test

Run in the xv6 shell:

```text
paging_sim
```

### Observed Results

| Frames | Algorithm | Page Faults | Hits | Hit Ratio |
|---:|---|---:|---:|---:|
| 3 | FIFO | 15 | 5 | 25% |
| 3 | LRU | 12 | 8 | 40% |
| 4 | FIFO | 10 | 10 | 50% |
| 4 | LRU | 8 | 12 | 60% |
| 5 | FIFO | 9 | 11 | 55% |
| 5 | LRU | 7 | 13 | 65% |

See [`Q3.png`](Q3.png).

## Build and Run

Use the xv6 working directory to build and launch the system:

```bash
cd ~/Desktop/xv6-vm-assignment
make clean
make
make qemu-nox
```

At the xv6 shell prompt, run the relevant test programs:

```text
pttest
lazytest
paging_sim
```

## Source Patches

Each question's `diffs/` directory contains source patches documenting its implementation changes. The test programs and question-specific READMEs are included alongside those patches.

## Test Evidence

- `Q1.png` — Page-table mappings and allocation statistics.
- `Q2.png` — Lazy-allocation page-fault counts and invalid-access test.
- `Q3.png` — FIFO and LRU results for 3, 4, and 5 frames.

## Notes

- The untouched clean xv6 source tree is kept separate from the working directory.
- Testing was performed in xv6 running under QEMU.
- The reported results reflect the tests executed; they do not imply exhaustive testing of every possible edge case.

