
#include "types.h"
#include "stat.h"
#include "user.h"

#define NREF 20
#define MAX_FRAMES 5

int reference[NREF] = {
  7, 0, 1, 2, 0, 3, 0, 4, 2, 3,
  0, 3, 2, 1, 2, 0, 1, 7, 0, 1
};

// Display the current contents of the frames.
void
printframes(int frames[], int nframes)
{
  int j;

  printf(1, "[ ");

  for(j = 0; j < nframes; j++){
    if(frames[j] == -1)
      printf(1, "- ");
    else
      printf(1, "%d ", frames[j]);
  }

  printf(1, "]");
}

// Print summary statistics.
void
printsummary(char *name, int nframes, int faults)
{
  int hits;

  hits = NREF - faults;

  printf(1, "\n%s Summary (%d frames):\n", name, nframes);
  printf(1, "Page Faults: %d\n", faults);
  printf(1, "Page Hits: %d\n", hits);
  printf(1, "Hit Ratio: %d%%\n", (hits * 100) / NREF);
  printf(1, "Page Fault Ratio: %d%%\n",
         (faults * 100) / NREF);
}

// FIFO Page Replacement.
int
fifo(int nframes, int display)
{
  int frames[MAX_FRAMES];
  int i, j, next, hit, faults;

  for(i = 0; i < nframes; i++)
    frames[i] = -1;

  next = 0;
  faults = 0;

  if(display)
    printf(1, "\n--- FIFO (%d Frames) ---\n", nframes);

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

    if(display){
      printf(1, "Page %d: ", reference[i]);
      printframes(frames, nframes);
      printf(1, "  %s\n", hit ? "HIT" : "FAULT");
    }
  }

  if(display)
    printsummary("FIFO", nframes, faults);

  return faults;
}

// LRU Page Replacement.
int
lru(int nframes, int display)
{
  int frames[MAX_FRAMES];
  int last_used[MAX_FRAMES];
  int i, j, victim, hit, faults;

  for(i = 0; i < nframes; i++){
    frames[i] = -1;
    last_used[i] = -1;
  }

  faults = 0;

  if(display)
    printf(1, "\n--- LRU (%d Frames) ---\n", nframes);

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
      victim = -1;

      // Prefer an empty frame if one exists.
      for(j = 0; j < nframes; j++){
        if(frames[j] == -1){
          victim = j;
          break;
        }
      }

      // Otherwise replace the least recently used page.
      if(victim == -1){
        victim = 0;

        for(j = 1; j < nframes; j++){
          if(last_used[j] < last_used[victim])
            victim = j;
        }
      }

      frames[victim] = reference[i];
      last_used[victim] = i;
      faults++;
    }

    if(display){
      printf(1, "Page %d: ", reference[i]);
      printframes(frames, nframes);
      printf(1, "  %s\n", hit ? "HIT" : "FAULT");
    }
  }

  if(display)
    printsummary("LRU", nframes, faults);

  return faults;
}

// Optimal Page Replacement.
int
optimal(int nframes, int display)
{
  int frames[MAX_FRAMES];
  int i, j, k, victim;
  int hit, faults, farthest, next_use;

  for(i = 0; i < nframes; i++)
    frames[i] = -1;

  faults = 0;

  if(display)
    printf(1, "\n--- OPTIMAL (%d Frames) ---\n", nframes);

  for(i = 0; i < NREF; i++){
    hit = 0;

    // Check whether the requested page is already present.
    for(j = 0; j < nframes; j++){
      if(frames[j] == reference[i]){
        hit = 1;
        break;
      }
    }

    if(!hit){
      victim = -1;

      // Use an empty frame if available.
      for(j = 0; j < nframes; j++){
        if(frames[j] == -1){
          victim = j;
          break;
        }
      }

      // If all frames are occupied, find the optimal victim.
      if(victim == -1){
        farthest = -1;
        victim = 0;

        for(j = 0; j < nframes; j++){
          next_use = NREF + 1;

          for(k = i + 1; k < NREF; k++){
            if(reference[k] == frames[j]){
              next_use = k;
              break;
            }
          }

          // Prefer a page that is never used again.
          if(next_use == NREF + 1){
            victim = j;
            break;
          }

          if(next_use > farthest){
            farthest = next_use;
            victim = j;
          }
        }
      }

      frames[victim] = reference[i];
      faults++;
    }

    if(display){
      printf(1, "Page %d: ", reference[i]);
      printframes(frames, nframes);
      printf(1, "  %s\n", hit ? "HIT" : "FAULT");
    }
  }

  if(display)
    printsummary("OPTIMAL", nframes, faults);

  return faults;
}

int
main(void)
{
  int frames;
  int ffaults, lfaults, ofaults;

  printf(1, "\n========== Q4: Page Replacement Algorithms ==========\n");

  printf(1, "Reference String: ");

  for(frames = 0; frames < NREF; frames++)
    printf(1, "%d ", reference[frames]);

  printf(1, "\nTotal References: %d\n", NREF);

  // Display detailed execution for all algorithms with 3 frames.
  fifo(3, 1);
  lru(3, 1);
  optimal(3, 1);

  // Compare all algorithms with 3, 4 and 5 frames.
  printf(1, "\n========== COMPARISON TABLE ==========\n");
  printf(1, "Frames  Algorithm  Faults  Hits  Hit%%  Fault%%\n");

  for(frames = 3; frames <= 5; frames++){
    ffaults = fifo(frames, 0);
    lfaults = lru(frames, 0);
    ofaults = optimal(frames, 0);

    printf(1, "%d       FIFO       %d       %d     %d%%    %d%%\n",
           frames, ffaults, NREF - ffaults,
           ((NREF - ffaults) * 100) / NREF,
           (ffaults * 100) / NREF);

    printf(1, "%d       LRU        %d       %d     %d%%    %d%%\n",
           frames, lfaults, NREF - lfaults,
           ((NREF - lfaults) * 100) / NREF,
           (lfaults * 100) / NREF);

    printf(1, "%d       OPTIMAL    %d       %d     %d%%    %d%%\n",
           frames, ofaults, NREF - ofaults,
           ((NREF - ofaults) * 100) / NREF,
           (ofaults * 100) / NREF);
  }

  printf(1, "\n========== Q4 Test Completed ==========\n");

  exit();
}

