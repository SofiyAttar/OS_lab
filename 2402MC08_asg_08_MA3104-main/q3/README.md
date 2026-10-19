## Assignment 8 — Q3: Page Table Inspection in xv6

### Kernel-Side Implementation

The `vmprint(pgdir)` function is implemented in `vm.c` to traverse and display the page table of a process at both levels of the hierarchy. It provides details about page mappings, physical frames, and access permissions.

**1. Page Directory Entries (Level 1)**

The function iterates through the page directory and identifies present page directory entries (PDEs). For each entry, it prints the index, the physical address of the associated page table using `PTE_ADDR`, and the entry's flags.

**2. Page Table Entries (Level 2)**

For each present PDE below `KERNBASE`, the function examines its page table and prints the following information for every present page table entry (PTE):

- Entry index within the page table.
- Corresponding virtual address, calculated as `(pdx << 22) | (ptx << 12)`.
- Physical address of the mapped frame.
- Permission flags: `P` (present), `W` (writable), and `U` (user-accessible).

The output uses indentation to distinguish page table entries from their parent directory entries. Kernel-space PDEs at or above `KERNBASE` are counted rather than expanded, and a summary line reports the number of kernel entries.

**3. Identifying the Stack Guard Page**

During `exec()`, xv6 creates a guard page immediately below the user stack by clearing the `PTE_U` flag through `clearpteu()`. Although this page remains mapped, it cannot be accessed from user mode.

The dump explicitly labels this entry as:

`<== guard page (mapped, U cleared)`

**4. Verifying Page Counts**

A final validation line compares the number of user-accessible pages against `sz/PGSIZE - 1`, excluding the guard page. When the values agree, the output reports `MATCH`, confirming that the page count is consistent with the process's memory layout.

### User-Side Testing (`vmprinttest.c`)

The test program invokes the page table dump at different stages to verify that the displayed mappings change correctly as the process's memory size changes.

#### Observed Results

The output is captured in `q3_vmprinttest.png`.

1. **Initial state:** The dump displays PDE 0 and its user-space mappings for the program's text, data, heap, and stack. The guard page is marked separately. Kernel-space PDEs, beginning at index 512, are summarized, and the page-count verification reports `MATCH`.

2. **After memory allocation:** Calling `sbrk(8 * 4096)` adds eight heap pages. The corresponding PTEs appear under PDE 0 in the subsequent dump.

3. **After memory deallocation:** Calling `sbrk(-4096)` releases one heap page. Its corresponding PTE disappears from the dump as `deallocuvm()` reclaims the page.

The results show that the page table hierarchy, entry indentation, address mappings, and page counts remain consistent throughout these operations.

### Conclusion

The implementation provides a structured view of the process's virtual memory mappings and distinguishes user-space entries from kernel-space mappings. The tests also demonstrate that page table entries are updated appropriately when memory is allocated or released, while the guard page remains identifiable through its access permissions.

### Build and Execution

Build xv6 and launch QEMU:

```bash
make clean && make qemu-nox