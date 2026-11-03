# Operating Systems — Assignment 10 Report

**Name:** Sofiya Attar  
**Roll Number:** 2401MC24

## 1. Introduction

This assignment focuses on memory management, virtual memory translation, page protection and page replacement algorithms in xv6. The objective was to implement and test additional memory-management functionalities through kernel modifications and user-level programs.

All implementations were tested using xv6 running under QEMU.

## 2. Question 1 — Tracking Physical Memory Allocation

The physical memory allocator in xv6 was modified to track page allocation and deallocation.

Two counters, `total_allocated` and `total_freed`, were introduced in the allocator's `kmem` structure. The `kalloc()` function increments the allocation counter whenever a physical page is successfully allocated, while `kfree()` increments the freed-page counter whenever a page is released.

A helper function, `getmemstats()`, retrieves these counters and calculates the number of available physical pages by traversing the free list. The `memstats()` system call displays total allocated pages, total freed pages, currently allocated pages and available physical memory pages.

The user program `memtest.c` allocates four pages using `sbrk()`, accesses them, displays the updated statistics and subsequently releases the allocated memory.

**Observation:** The allocation counter increased from 425 to 429, while the freed counter increased from 214 to 218 after deallocation. The currently allocated pages returned from 215 to 211, and available physical pages returned from 56,786 to 56,790.

This verified that allocation and deallocation were tracked correctly.

## 3. Question 2 — Virtual-to-Physical Address Translation

A `vtop()` helper function was implemented in `vm.c` to translate virtual addresses into physical addresses.

The implementation uses `walkpgdir()` to locate the corresponding page-table entry and checks whether the entry is present using `PTE_P`. The physical frame address is extracted using `PTE_ADDR()`, and the virtual page offset is preserved to calculate the complete physical address.

A `vtop()` system call was implemented to allow user programs to request address translation. Invalid or unmapped addresses return `-1`.

The user program `vttest.c` tests a valid virtual address, another address within the same page with a 100-byte offset, and an unmapped address.

**Observation:** Virtual address `0x3000` translated to physical address `0xDFBC000`. Virtual address `0x3064` translated to `0xDFBC064`, correctly preserving the 100-byte offset. The unmapped address `0x4000` was correctly detected.

## 4. Question 3 — Page Table Management and Page Protection

Two helper functions, `pageinfo()` and `setreadonly()`, were implemented in `vm.c`.

The `pageinfo()` function checks whether a virtual address is mapped and displays its physical frame address and page-table flags: P (Present), W (Writable) and U (User).

The `setreadonly()` function removes write permission by clearing the `PTE_W` bit in the corresponding page-table entry. The page directory is reloaded using `lcr3()` to flush the TLB and ensure that the updated permissions take effect.

Two system calls, `pageinfo()` and `setreadonly()`, were implemented and registered.

The user program `pagetest.c` allocates a page, displays its initial permissions, changes it to read-only and attempts to write to it from a child process.

**Observation:** Initially, the page had P=1, W=1 and U=1. After changing it to read-only, the flags became P=1, W=0 and U=1. The child's write attempt generated a page fault (trap 14, error code 7), and the child was terminated while the parent process continued successfully.

This verified that page protection was enforced correctly.

## 5. Question 4 — Page Replacement Algorithms

Three page replacement algorithms were implemented in the user program `pagereplace.c`:

- **FIFO (First-In First-Out):** Replaces the page that entered memory earliest.
- **LRU (Least Recently Used):** Replaces the page that has not been accessed for the longest time.
- **Optimal:** Replaces the page whose next reference is farthest in the future, or which will not be referenced again.

All algorithms were tested using the reference string:

`7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`

The algorithms were evaluated using 3, 4 and 5 frames. The program displays frame contents and indicates hits and faults for every reference when using three frames. It also calculates the total page faults, hits, hit ratio and page fault ratio for all configurations.

**Observation:**

- With 3 frames, FIFO produced 15 faults, LRU produced 12 faults and Optimal produced 9 faults.
- With 4 frames, FIFO produced 10 faults, while LRU and Optimal each produced 8 faults.
- With 5 frames, FIFO produced 9 faults, while LRU and Optimal each produced 7 faults.

Optimal achieved the minimum number of page faults for the given reference string. LRU performed better than FIFO in all three configurations.

## 6. Conclusion

All four questions were successfully implemented and tested in xv6 under QEMU. The assignment provided practical experience with physical memory allocation, page-table traversal, virtual-to-physical address translation, page permissions and page replacement strategies.

The test results verified the correctness of the implemented functionalities for the specified test cases. The modified source files, user programs, patches and terminal screenshots are included in the submission.


