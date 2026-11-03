
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096
#define NUM_PAGES 4

int
main(void)
{
  char *mem;

  printf(1, "\n========== Q1: Physical Memory Tracking ==========\n");

  // Display memory statistics before allocation.
  printf(1, "\n--- Initial Memory Statistics ---\n");
  memstats();

  // Allocate four pages using sbrk().
  printf(1, "\nAllocating %d pages...\n", NUM_PAGES);

  mem = sbrk(NUM_PAGES * PGSIZE);

  if(mem == (char*)-1){
    printf(1, "Memory allocation failed\n");
    exit();
  }

  // Touch the allocated pages.
  // This ensures the allocated memory is actually used.
  mem[0] = 'A';
  mem[PGSIZE] = 'B';
  mem[2 * PGSIZE] = 'C';
  mem[3 * PGSIZE] = 'D';

  printf(1, "Successfully allocated %d pages.\n", NUM_PAGES);

  // Display statistics after allocation.
  printf(1, "\n--- After Allocation ---\n");
  memstats();

  // Release the allocated memory.
  printf(1, "\nFreeing %d pages...\n", NUM_PAGES);

  if(sbrk(-NUM_PAGES * PGSIZE) == (char*)-1){
    printf(1, "Memory deallocation failed\n");
    exit();
  }

  printf(1, "Memory released successfully.\n");

  // Display statistics after deallocation.
  printf(1, "\n--- After Deallocation ---\n");
  memstats();

  printf(1, "\n========== Test Completed ==========\n");

  exit();
}

