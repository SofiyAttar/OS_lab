# Q4 — Page Replacement Algorithms: FIFO, LRU and Optimal

**Name:** Sofiya Attar  
**Roll Number:** 2401MC24

## Objective

Implement and compare three page replacement algorithms in xv6 using a fixed reference string and different numbers of available frames.

## Reference String

`7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`

Total number of page references: 20.

## Implementation

Implemented the following algorithms in `pagereplace.c`:

### 1. FIFO — First-In First-Out

Replaces the page that has been in memory for the longest time, based on its arrival order.

### 2. LRU — Least Recently Used

Replaces the page that has not been accessed for the longest period of time. The implementation maintains the last-used index of each page.

### 3. Optimal Page Replacement

Replaces the page whose next reference is farthest in the future. A page that will not be referenced again is preferred for replacement.

The program displays the frame contents and indicates HIT or FAULT for every page reference when using three frames. It also calculates summary statistics for all three algorithms with 3, 4 and 5 frames.

## Test Results

The program was executed successfully in xv6 running under QEMU.

| Frames | Algorithm | Page Faults | Page Hits | Hit Ratio | Fault Ratio |
|---:|---|---:|---:|---:|---:|
| 3 | FIFO | 15 | 5 | 25% | 75% |
| 3 | LRU | 12 | 8 | 40% | 60% |
| 3 | Optimal | 9 | 11 | 55% | 45% |
| 4 | FIFO | 10 | 10 | 50% | 50% |
| 4 | LRU | 8 | 12 | 60% | 40% |
| 4 | Optimal | 8 | 12 | 60% | 40% |
| 5 | FIFO | 9 | 11 | 55% | 45% |
| 5 | LRU | 7 | 13 | 65% | 35% |
| 5 | Optimal | 7 | 13 | 65% | 35% |

## Observations

- FIFO produces the highest number of page faults among the three algorithms for this reference string.
- LRU performs better than FIFO.
- Optimal produces the minimum number of page faults.
- With four frames, LRU and Optimal both produce 8 page faults.
- With five frames, LRU and Optimal both produce 7 page faults.
- Increasing the number of frames improves or maintains the hit ratio for these algorithms on the given reference string.

## Test Evidence

- `../Q4_part1.png` — FIFO execution with three frames.
- `../Q4_part2.png` — LRU and Optimal execution with three frames.
- `../Q4_part3.png` — Comparison table for 3, 4 and 5 frames.

## Conclusion

FIFO, LRU and Optimal page replacement algorithms were successfully implemented and tested. The results demonstrate the differences in page replacement strategies and show that Optimal achieves the minimum number of page faults for the given reference string.
