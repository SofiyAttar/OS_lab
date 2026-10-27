# Q3 — Page Replacement Algorithms

## Objective
Implement and compare FIFO (First-In, First-Out) and LRU (Least Recently Used) page replacement algorithms using a fixed reference string and different frame counts.

## Reference String
`7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`

Total references: 20

## Implementation
- Implemented FIFO using a circular replacement index.
- Implemented LRU using the most recent reference position of each page.
- Simulated both algorithms with 3, 4, and 5 frames.
- Reported page faults, hits, and integer hit ratio percentages.

## Results

| Frames | Algorithm | Page Faults | Hits | Hit Ratio |
|---:|---|---:|---:|---:|
| 3 | FIFO | 15 | 5 | 25% |
| 3 | LRU | 12 | 8 | 40% |
| 4 | FIFO | 10 | 10 | 50% |
| 4 | LRU | 8 | 12 | 60% |
| 5 | FIFO | 9 | 11 | 55% |
| 5 | LRU | 7 | 13 | 65% |

## Running the Test
Run the following command in the xv6 shell:

```text
paging_sim
```

## Test Evidence
See `../Q3.png` for the successful test output.

## Source Changes
The `diffs/` directory contains the Q3-specific source patches.

