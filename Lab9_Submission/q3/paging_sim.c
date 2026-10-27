
#include "types.h"
#include "stat.h"
#include "user.h"

#define NREF 20
#define MAX_FRAMES 5

int reference[NREF] = {
  7, 0, 1, 2, 0, 3, 0, 4, 2, 3,
  0, 3, 2, 1, 2, 0, 1, 7, 0, 1
};

int
fifo(int nframes)
{
  int frames[MAX_FRAMES];
  int i, j, faults = 0, next = 0, hit;

  for(i = 0; i < nframes; i++)
    frames[i] = -1;

  for(i = 0; i < NREF; i++){
    hit = 0;

    for(j = 0; j < nframes; j++){
      if(frames[j] == reference[i]){
        hit = 1;
        break;
      }
    }

    if(!hit){
      frames[next] = reference[i];
      next = (next + 1) % nframes;
      faults++;
    }
  }

  printf(1, "FIFO with %d frames: faults=%d, hits=%d, hit ratio=%d%%\n",
         nframes, faults, NREF - faults,
         ((NREF - faults) * 100) / NREF);

  return faults;
}

int
lru(int nframes)
{
  int frames[MAX_FRAMES];
  int last_used[MAX_FRAMES];
  int i, j, victim, faults = 0, hit;

  for(i = 0; i < nframes; i++){
    frames[i] = -1;
    last_used[i] = -1;
  }

  for(i = 0; i < NREF; i++){
    hit = 0;

    for(j = 0; j < nframes; j++){
      if(frames[j] == reference[i]){
        hit = 1;
        last_used[j] = i;
        break;
      }
    }

    if(!hit){
      victim = 0;

      for(j = 0; j < nframes; j++){
        if(frames[j] == -1){
          victim = j;
          break;
        }
        if(last_used[j] < last_used[victim])
          victim = j;
      }

      frames[victim] = reference[i];
      last_used[victim] = i;
      faults++;
    }
  }

  printf(1, "LRU  with %d frames: faults=%d, hits=%d, hit ratio=%d%%\n",
         nframes, faults, NREF - faults,
         ((NREF - faults) * 100) / NREF);

  return faults;
}

int
main(void)
{
  int frames;

  printf(1, "Q3: Page Replacement Simulation\n");
  printf(1, "Reference string has %d pages\n", NREF);

  for(frames = 3; frames <= 5; frames++){
    fifo(frames);
    lru(frames);
  }

  exit();
}

