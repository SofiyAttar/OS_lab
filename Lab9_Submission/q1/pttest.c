
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

int
main(void)
{
  char *oldbrk;
  char *newbrk;

  printf(1, "Q1: Page table before allocation\n");
  vmprint();

  oldbrk = sbrk(4 * PGSIZE);
  if(oldbrk == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  // Touch each new page to verify its mapping.
  oldbrk[0] = 1;
  oldbrk[PGSIZE] = 2;
  oldbrk[2 * PGSIZE] = 3;
  oldbrk[3 * PGSIZE] = 4;

  printf(1, "\nQ1: Page table after allocating four pages\n");
  vmprint();

  newbrk = sbrk(0);
  printf(1, "Heap growth: %d bytes\n", newbrk - oldbrk);

  exit();
}

