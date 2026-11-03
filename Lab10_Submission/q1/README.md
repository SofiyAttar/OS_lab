# Q1 — Tracking Physical Memory Allocation

**Name:** Sofiya Attar  
**Roll Number:** 2401MC24

## Objective

Implement a mechanism in xv6 to track physical memory page allocation and deallocation using the kernel page allocator functions `kalloc()` and `kfree()`.

## Implementation

### 1. Allocation and Deallocation Counters

Modified `kalloc.c` to maintain two counters:

- `total_allocated`: Tracks the total number of physical pages allocated.
- `total_freed`: Tracks the total number of physical pages freed.

Both counters are protected using the allocator's existing spinlock.

The counters are reset after the initial free-memory list is populated during `kinit2()`, so the statistics represent runtime allocation activity.

### 2. Memory Statistics Function

Implemented `getmemstats()` to retrieve:

- Total allocated pages.
- Total freed pages.
- Available physical memory pages.

Available pages are calculated by traversing the allocator's free list.

### 3. System Call

Implemented the `memstats()` system call to display:

- Total allocated pages.
- Total freed pages.
- Currently allocated pages.
- Available physical memory pages.

Currently allocated pages are calculated as:

`Total allocated pages - Total freed pages`

### 4. User Test Program

Created `memtest.c` to:

1. Display initial memory statistics.
2. Allocate four pages using `sbrk()`.
3. Access the allocated pages.
4. Display updated memory statistics.
5. Release the four pages using `sbrk()`.
6. Display the final memory statistics.

## Files Modified

- `kalloc.c`
- `defs.h`
- `sysproc.c`
- `syscall.h`
- `syscall.c`
- `user.h`
- `usys.S`
- `Makefile`

## Test Results

The program was executed successfully in xv6 running under QEMU.

| Statistic | Before allocation | After allocation | After deallocation |
|---|---:|---:|---:|
| Total allocated pages | 425 | 429 | 429 |
| Total freed pages | 214 | 214 | 218 |
| Currently allocated pages | 211 | 215 | 211 |
| Available pages | 56,790 | 56,786 | 56,790 |

Four pages were successfully allocated and subsequently released. The available physical memory returned to its initial value.

## Test Evidence

See `../Q1.png` for the terminal output demonstrating successful execution of `memtest`.

## Conclusion

The physical memory tracking mechanism was successfully implemented and tested. The allocation and deallocation counters correctly reflected the four-page allocation and release, and the available physical memory returned to its initial value after deallocation.
