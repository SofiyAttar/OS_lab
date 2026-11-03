
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(void)
{
  char *mem;
  int pa1, pa2, pa3;
  uint va1, va2, invalid_va;

  printf(1, "\n========== Q2: Virtual-to-Physical Translation ==========\n");

  // Allocate one page of user memory.
  mem = sbrk(PGSIZE);

  if(mem == (char*)-1){
    printf(1, "Memory allocation failed\n");
    exit();
  }

  // Touch the page to ensure it is mapped.
  mem[0] = 'A';

  // Test 1: Translate the first byte of the allocated page.
  va1 = (uint)mem;
  pa1 = vtop((int)va1);

  printf(1, "\n--- Test 1: Valid Virtual Address ---\n");
  printf(1, "Virtual address: 0x%x\n", va1);

  if(pa1 == -1)
    printf(1, "Translation failed\n");
  else
    printf(1, "Physical address: 0x%x\n", (uint)pa1);

  // Test 2: Translate another address within the same page.
  va2 = va1 + 100;
  pa2 = vtop((int)va2);

  printf(1, "\n--- Test 2: Address with Page Offset ---\n");
  printf(1, "Virtual address: 0x%x\n", va2);

  if(pa2 == -1)
    printf(1, "Translation failed\n");
  else {
    printf(1, "Physical address: 0x%x\n", (uint)pa2);
    printf(1, "Physical offset: %d\n",
           (uint)pa2 & (PGSIZE - 1));
  }

  // Test 3: Translate an address in the next, unmapped page.
  invalid_va = va1 + PGSIZE;
  pa3 = vtop((int)invalid_va);

  printf(1, "\n--- Test 3: Invalid / Unmapped Address ---\n");
  printf(1, "Virtual address: 0x%x\n", invalid_va);

  if(pa3 == -1)
    printf(1, "Correctly detected unmapped address.\n");
  else
    printf(1, "Unexpected translation: 0x%x\n", (uint)pa3);

  // Release the allocated page.
  sbrk(-PGSIZE);

  printf(1, "\n========== Q2 Test Completed ==========\n");

  exit();
}

