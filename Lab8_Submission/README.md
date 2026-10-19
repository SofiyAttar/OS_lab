# Assignment 8: Virtual Memory Management in xv6

**Name:** Sofiya Ejaj Attar  
**Roll No.:** 2401MC24

## Introduction

This assignment explores virtual memory management in xv6 through four extensions. The implementations cover address translation, physical memory monitoring, page table inspection, and lazy allocation. Each feature is accompanied by test programs and supporting files to demonstrate its behavior.

## Implemented Features

### Q1 — Virtual-to-Physical Address Translation

A new system call, `sys_va2pa`, is implemented to examine how virtual addresses are mapped to physical memory. It uses `walkpgdir` to locate the relevant page table entry and extracts the PDX, PTX, offset, and page flags.

The test also examines memory mapping across `fork()`, demonstrating how the parent and child can use the same virtual address while referring to different physical pages.

### Q2 — Tracking Physical Memory Usage

The memory allocator in `kalloc.c` is extended to maintain the number of available physical pages. A `getmeminfo` system call allows user programs to retrieve memory statistics.

The implementation is tested by observing memory usage before and after operations such as `sbrk()` and `fork()`. After the child process terminates and the parent calls `wait()`, the free-page count returns to its initial value, indicating that the allocated pages have been reclaimed.

### Q3 — Inspecting the Page Table

The `vmprint` functionality traverses the page directory and displays its entries in a hierarchical format. User-space page table entries are printed with indentation, while kernel-space mappings are summarized.

The output also helps verify page-level access permissions. In particular, the stack guard page can be identified by the absence of the `PTE_U` flag, which prevents user-mode access to that page.

### Q4 — Demand Paging Through Lazy Allocation

Lazy allocation changes the way xv6 handles requests for additional process memory. Instead of allocating physical pages immediately, `sys_sbrk` updates the process's size and postpones allocation until the memory is accessed.

When a page fault (`T_PGFLT`) occurs, the trap handler calls `lazyalloc` to allocate a zero-filled physical page and establish the required mapping. The `copyuvm` function is also modified to account for pages that have not yet been allocated.

The test requests 32 MB of additional memory and checks that physical frames are not consumed for the untouched region. Pages are allocated only as the process accesses them.

## Repository Layout

The assignment is organized into separate directories for each question.

| Directory | Contents |
|---|---|
| `q1/` | `vatest.c`, implementation diffs, and test screenshot |
| `q2/` | `memtest.c`, `meminfo.h`, implementation diffs, and test screenshot |
| `q3/` | `vmprinttest.c`, implementation diffs, and test screenshot |
| `q4/` | `lazytest.c`, `meminfo.h`, implementation diffs, and test screenshot |

Each directory contains the relevant test program and supporting material for its respective implementation.