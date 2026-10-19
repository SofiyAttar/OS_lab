## Assignment 8 — Q2: Physical Memory Statistics in xv6

### Kernel-Side Implementation

The physical memory allocator is instrumented to track memory usage and report page-level statistics.

**1. Changes to `kalloc.c`**

Two counters are added to the `kmem` structure and maintained under `kmem.lock`:

- `free_pages`: Decremented whenever `kalloc()` allocates a page and incremented when `kfree()` releases one.
- `total_pages`: Initialized once at the end of `kinit2()`, after the complete free list has been constructed. The current build reports 57,001 total pages.

The functions `nfree_pages()` and `ntotal_pages()` provide access to these counters while holding the lock, ensuring consistent readings.

**2. Page Table Statistics in `vm.c`**

The function `count_pt_pages(pgdir)` calculates the number of page-table-related pages associated with a process. It counts the page directory itself and one page for each present page directory entry (PDE), since each such entry points to a page table.

Every process also contains page-table mappings for the kernel region. Consequently, even a small user process reports approximately 66 page-table pages. This is expected behavior rather than evidence of a memory leak.

**3. Memory Information Structure**

The header `meminfo.h` defines a shared structure for reporting memory statistics:

- `total_pages` — Total physical pages available to the allocator.
- `free_pages` — Currently available physical pages.
- `user_pages` — Number of pages represented by the process's memory size.
- `pt_pages` — Number of page-directory and page-table pages.

The header uses include guards and is shared between kernel and user-space code.

**4. `getmeminfo` System Call**

The `getmeminfo(mi)` system call is implemented in `sysproc.c` and registered as syscall number `22`. It validates the user-provided pointer using `argptr()` and fills the structure with the total and free page counts, the process size divided by `PGSIZE`, and the result of `count_pt_pages(cur->pgdir)`.

### User-Side Testing (`memtest.c`)

The test program records memory statistics at different stages of execution to verify allocation, process creation, and memory reclamation.

#### Observed Results

The following values were recorded in `q2_memtest.png`.

| Test stage | Total pages | Free pages | User pages | Page-table pages |
|---|---:|---:|---:|---:|
| Initial state | 57,001 | 56,790 | 3 | 66 |
| After `sbrk()` allocates 8 pages | 57,001 | 56,782 | 11 | 66 |

After the `sbrk()` operation, the free-page count decreases by exactly eight, while the reported user memory increases by eight pages.

When both the parent and child processes are alive, the free-page count drops further to `56,704`. This represents a decrease of 78 pages from the pre-fork value. The additional consumption includes the child's copied user pages and its page-directory and page-table pages.

Once the child exits and the parent reaps it using `wait()`, the free-page count returns to `56,782`, matching the value recorded before `fork()`.

The parent and child may print their statistics concurrently, causing their output lines to interleave. This is a console-output effect and does not indicate a memory-management error.

### Conclusion

The test results demonstrate that physical memory statistics reflect page allocation and reclamation. The free-page count decreases when additional memory is allocated and when a process is forked. After the child terminates and is reaped, the count returns to its pre-fork value.

This confirms that the tested process cleanup path correctly reclaims the allocated pages, with no net loss of free pages across the fork-and-reap cycle.

### Build and Execution

Build xv6 and launch the QEMU environment:

```bash
make clean && make qemu-nox