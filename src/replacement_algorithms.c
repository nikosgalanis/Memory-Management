#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/replacement_algorithms.h"

/* Function that returns the position of the main memory that needs to be
   sacrifised, given an alforithm and the main memory */
int find_page_to_sacrifise(int algorithm, main_memory_entry* main_memory, int size) {
    if (algorithm == LRU)
      return lru(main_memory, size);
    // else if (algorithm == WS)
    //   return ws(main_memory, size);
}

/* Implementation of the LRU algorithm. We sacrifice the least recently used
  page, denoted by a time signature */
int lru(main_memory_entry* main_memory, int size) {
  int pos = -1;
  int min = time;
  /* Traverse the main memory to find the page with the smaller time_signature */
  for (size_t i = 0; i < size; ++i) {
    if (main_memory[i].time_signature < min) {
      min = main_memory[i].time_signature;
      pos = i;
    }
  }
  /* Simulation of "writing to the disk", by setting the soon to be removed page
    as dirty */
  if (main_memory[pos].has_writen == True) {
    dirty_pages++;
  }
  
  if (pos == -1) {
    printf("Something unexpected happened during page replacement. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  return pos;
}
