
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096
#define NPAGES 32

int
main(void)
{
  char *mem;
  int faults_before;
  int faults_after;
  int i;
  int pid;
  int status;

  printf(1, "Q2: Lazy Allocation Test\n");

  faults_before = getfaults();

  // Reserve 32 pages without touching them.
  mem = sbrk(NPAGES * PGSIZE);
  if(mem == (char*)-1){
    printf(1, "sbrk failed\n");
    exit();
  }

  faults_after = getfaults();
  printf(1, "Faults after reservation: %d\n",
         faults_after - faults_before);

  // Touch five distinct pages.
  for(i = 0; i < 5; i++)
    mem[i * PGSIZE] = i + 1;

  faults_after = getfaults();
  printf(1, "Faults after touching 5 pages: %d\n",
         faults_after - faults_before);

  // Test an address outside the reserved range in a child.
  pid = fork();
  if(pid == 0){
    mem[NPAGES * PGSIZE] = 99;
    printf(1, "ERROR: invalid access was not stopped\n");
    exit();
  } else if(pid > 0){
    wait();
    printf(1, "Parent survived invalid child access\n");
  } else {
    printf(1, "fork failed\n");
  }

  // Read status to avoid an unused-variable warning if needed.
  status = 0;
  (void)status;

  exit();
}

