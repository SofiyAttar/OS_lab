# Q2 — Virtual-to-Physical Address Translation

**Name:** Sofiya Attar  
**Roll Number:** 2401MC24

## Objective

Implement a system call `vtop()` in xv6 to translate a given virtual address into its corresponding physical address using the process's page table.

## Implementation

### 1. Virtual-to-Physical Translation

Implemented the `vtop()` helper function in `vm.c`.

- Used `walkpgdir()` to locate the page-table entry corresponding to the given virtual address.
- Checked whether the page-table entry exists and is present using `PTE_P`.
- Extracted the physical frame address using `PTE_ADDR()`.
- Preserved the page offset while calculating the complete physical address.
- Returned zero for invalid or unmapped addresses.

The translation is calculated as:

`Physical Address = Physical Frame Address + Page Offset`

### 2. System Call

Implemented the `vtop()` system call through `sys_vtop()`.

The handler retrieves the virtual address from the user program, accesses the current process's page directory, and invokes the translation helper.

It returns `-1` if the address is invalid or unmapped.

### 3. User Test Program

Created `vttest.c` to perform three tests:

1. Translate a valid virtual address.
2. Translate another address within the same page to verify page-offset preservation.
3. Translate an unmapped virtual address to demonstrate invalid-address handling.

## Files Modified

- `vm.c`
- `defs.h`
- `sysproc.c`
- `syscall.h`
- `syscall.c`
- `user.h`
- `usys.S`
- `Makefile`

## Test Results

The program was executed successfully in xv6 running under QEMU.

| Test | Virtual Address | Result |
|---|---|---|
| Valid address | `0x3000` | Physical address `0xDFBC000` |
| Address with offset | `0x3064` | Physical address `0xDFBC064` |
| Unmapped address | `0x4000` | Correctly detected as invalid |

The physical address for the second test preserved the 100-byte page offset.

## Test Evidence

See `../Q2.png` for the terminal output demonstrating successful execution of `vttest`.

## Conclusion

The virtual-to-physical address translation mechanism was successfully implemented and tested. Valid addresses were translated correctly, page offsets were preserved, and unmapped addresses were detected.
