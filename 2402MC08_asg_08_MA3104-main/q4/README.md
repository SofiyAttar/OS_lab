## Assignment 8 — Q4: Lazy Memory Allocation and Demand Paging in xv6

### Design Overview

In the standard xv6 implementation, `sbrk()` allocates physical memory when a process requests additional space. This implementation replaces eager allocation with **lazy allocation**, where the process's virtual address space is expanded immediately, but physical pages are allocated only when they are accessed.

This approach reduces unnecessary memory consumption and allows processes to reserve large regions of virtual memory without requiring the corresponding physical frames upfront.

### Kernel-Side Implementation

#### 1. Modifying `sys_sbrk()` in `sysproc.c`

The behavior of `sys_sbrk()` is modified to support deferred memory allocation.

- **Positive increment (`n > 0`):** Updates `cur->sz` without calling `growproc()` or allocating physical pages.
- **Negative increment (`n < 0`):** Uses `deallocuvm()` to release any physical pages that have already been allocated in the requested region.
- **Return value:** Returns the previous process size, which marks the beginning of the newly reserved region for a positive increment.

#### 2. Handling Page Faults in `trap.c`

The page fault handler is implemented under `T_PGFLT` to allocate memory when a process accesses an unmapped page.

The handler performs the following operations:

1. Retrieves the faulting virtual address using `rcr2()`.
2. Passes the address to `lazyalloc()` for validation and allocation.
3. Rejects addresses outside the process's allocated virtual range and accesses to guard pages, identified by a present PTE with `PTE_U` cleared.
4. Allocates a zero-initialized physical frame and maps it with the `PTE_W | PTE_U` permissions.
5. Returns from the trap handler so the CPU can retry the faulting instruction.

If the address is invalid or physical memory allocation fails, the process is marked for termination using `cur->killed = 1`. This ensures that an invalid memory access terminates the offending process rather than crashing the kernel.

#### 3. Virtual Memory Management Changes in `vm.c`

Several functions are updated to support processes containing virtual pages that have not yet been mapped to physical memory.

- **`lazyalloc(va)`:** Validates the faulting address, allocates a physical frame, initializes it to zero, and creates a user-accessible, writable mapping.
- **`copyuvm()`:** Skips page table entries that are not present instead of treating them as errors. This allows `fork()` to work when the parent contains unallocated lazy pages.
- **`deallocuvm()`:** Ignores absent page mappings because they do not have physical frames to release.
- **`freevm()`:** Handles partially populated address spaces and checks for a null page-directory pointer before proceeding.
- **`copyin()` and `copyout()`:** Are modified to handle accesses to lazy pages. When a kernel operation needs to access an unmapped page in a valid lazy region, the page is allocated before the copy continues.

These changes allow lazy allocation to work with process creation, memory deallocation, and system calls that transfer data between kernel and user space.

### User-Side Testing (`lazytest.c`)

The test program verifies that large virtual memory requests do not immediately consume physical memory, that page faults allocate memory on demand, and that lazy allocation remains compatible with other process operations.

#### Observed Results

The following results are recorded in `q4_lazytest.png`.

**1. Reserving 32 MB of Virtual Memory**

| Measurement | Before `sbrk()` | After `sbrk()` |
|---|---:|---:|
| Free physical pages | 56,790 | 56,790 |
| User pages | 3 | 8,195 |
| Page-table pages | 66 | 66 |

The process successfully reserves 32 MB of additional virtual memory without reducing the number of free physical pages. The increase in `user_pages` reflects the expanded virtual memory size, not the number of physical frames allocated.

**2. Accessing Selected Pages**

The test writes to three locations in the reserved region: the beginning, the midpoint at 16 MB, and the final byte.

After these accesses, the reported free-page count drops to `56,785`, while the page-table count increases to `68`. This corresponds to three data frames and two additional page-table frames being allocated.

Reading the locations back produces `A`, `B`, and `C`, confirming that the newly allocated pages retain the written values.

**3. Kernel Copy Operations**

The test also performs a `write()` operation using a buffer that has not previously been accessed. The operation succeeds because the kernel-side memory-copy path handles the lazy page and makes the required memory available. The eight bytes in the untouched buffer are initially zero.

**4. Forking a Process with Lazy Regions**

A child process is created while the parent still has unallocated pages in its virtual address space. The updated `copyuvm()` skips absent page table entries, allowing the fork operation to complete.

The child successfully reads the previously written value `B`, demonstrating that the existing mapped data remains accessible despite the presence of unallocated regions.

**5. Memory Reclamation After Fork**

After the child exits and is reaped, the test records:

- Free pages: `56,784`
- User pages: `8,195`
- Page-table pages: `66`

The parent retains the physical page allocated when its `write()` operation accessed the previously untouched buffer. Consequently, its free-page count does not return to the original pre-fork value.

**6. Invalid Memory Access**

Finally, the child attempts to access an address equal to the process size (`va == sz`). Since this address lies outside the valid virtual address range, the page fault handler terminates the child.

The log reports:

`pid 5: invalid page fault at va=2003000 - kill process`

The parent and kernel continue running normally, demonstrating that invalid accesses are isolated to the offending process.

### Conclusion

The implementation demonstrates demand paging by separating virtual memory reservation from physical memory allocation. A request for 32 MB succeeds without consuming additional data frames, and physical pages are allocated only when accessed.

The updated virtual memory routines also support partially populated address spaces, allowing `fork()`, memory reclamation, and kernel copy operations to work with lazy regions. Invalid page faults are handled by terminating the affected process without disrupting the kernel or other processes.

### Build and Execution

Compile xv6 and start QEMU:

```bash
make clean && make qemu-nox