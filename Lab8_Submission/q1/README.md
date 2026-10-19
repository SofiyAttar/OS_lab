## Assignment 8 — Q1: Virtual-to-Physical Address Translation in xv6

### Kernel-Side Implementation

The `walkpgdir()` function in `vm.c` is made accessible outside the file by removing its `static` qualifier and adding its prototype to `defs.h`. This allows the system call handler to inspect the current process's page table.

A new system call, `sys_va2pa(va)`, is implemented in `sysproc.c`. It performs the following operations:

- Uses `walkpgdir(cur->pgdir, va, 0)` to locate the page table entry corresponding to the supplied virtual address.
- Returns `-1` if the page table entry is absent (`PTE_P` is unset) or the page is not accessible from user space (`PTE_U` is unset). A diagnostic message is printed using `cprintf`.
- Calculates the physical address using `PTE_ADDR(*pte) + (va & 0xFFF)`.
- Prints the page directory index (PDX), page table index (PTX), 12-bit page offset, raw page table entry, permission flags (`P`, `W`, and `U`), and the resulting physical address.

The system call is registered as syscall number `22`, with the corresponding updates made to `syscall.h`, `syscall.c`, `usys.S`, and `user.h`.

### User-Side Testing (`vatest.c`)

The user program tests address translation using three types of memory:

- **Global variable:** Located in the process's data segment.
- **Local variable:** Stored on the user stack.
- **Dynamically allocated memory:** Obtained using `malloc()`.

For each variable, the program prints its virtual address and the corresponding physical address returned by `va2pa()`. The kernel also prints the associated page table information.

An invalid or unmapped address, `0x7FFFFFFF`, is tested separately. The system call returns `-1` when no valid user-accessible mapping exists.

The program also tests address translation across `fork()`. The parent and child processes use the same virtual address for `globalvar`, but the translation produces different physical addresses. This demonstrates xv6's process memory isolation, as `fork()` uses `copyuvm()` to create separate physical page copies for the child.

### Build and Execution

Compile the kernel and launch xv6 using:

```bash
make clean && make qemu-nox