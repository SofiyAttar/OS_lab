# Q3 — Page Table Management and Page Protection

**Name:** Sofiya Attar  
**Roll Number:** 2401MC24

## Objective

Implement mechanisms in xv6 to inspect page-table information and modify page permissions.

## Implementation

### 1. Page Information

Implemented the `pageinfo()` helper in `vm.c`.

- Uses `walkpgdir()` to locate the page-table entry.
- Checks whether the page is present using `PTE_P`.
- Extracts the physical frame address using `PTE_ADDR()`.
- Displays the P (Present), W (Writable), and U (User) flags.

### 2. Read-Only Page Protection

Implemented the `setreadonly()` helper in `vm.c`.

- Locates the page-table entry.
- Verifies that the page is present.
- Clears the `PTE_W` bit to remove write permission.
- Reloads the page directory using `lcr3()` to flush the TLB and apply the updated permissions.

### 3. System Calls

Implemented two system calls:

- `pageinfo()` — displays information about a mapped virtual page.
- `setreadonly()` — removes write permission from a mapped page.

### 4. User Test Program

Created `pagetest.c` to:

1. Allocate one page using `sbrk()`.
2. Access the page to ensure it is mapped.
3. Display its initial page-table information.
4. Change the page to read-only.
5. Display the updated page-table flags.
6. Fork a child process and attempt to write to the protected page.

The child process is expected to encounter a page fault, while the parent process continues execution.

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

| Test | Observed Result |
|---|---|
| Initial page information | P=1, W=1, U=1 |
| Set page read-only | Successful |
| Verify updated permissions | P=1, W=0, U=1 |
| Attempt write | Page fault, trap 14 |
| Parent process | Survived child termination |

The page-fault error code was 7, indicating a user-mode write protection violation on a present page.

## Test Evidence

See `../Q3.png` for the terminal output demonstrating successful execution of `pagetest`.

## Conclusion

Page-table inspection and read-only protection were successfully implemented and tested. The write permission was removed correctly, and an attempted write to the protected page triggered a page fault as expected.
