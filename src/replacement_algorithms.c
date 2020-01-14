#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/replacement_algorithms.h"

/* Function that returns the position of the main memory that needs to be
   sacrifised, given an alforithm and the main memory */
int find_page_to_sacrifise(int algorithm, ipt_entry* ipt, main_memory_entry* main_memory, int size, int pid, int window_size) {
    if (algorithm == LRU)
      return lru(main_memory, size);
    else if (algorithm == WS)
      return ws(ipt, main_memory, size, pid, window_size);
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
  if (main_memory[pos].modified == True) {
    dirty_pages++;
  }
  /* If we end up here, without sacrifising a page, something is definatelly
    wrond */
  if (pos == EMPTY) {
    printf("Something unexpected happened during page replacement. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  /* Return the position of the page that was chosen to be sacrificed */
  return pos;
}

/* Implementation of the WS algorithm. We keep a time frame of all the references,
  and then when a sacrifice is neccessary, we construct the set of this frame,
  and remove all of the pages that are not in that set.*/
int ws(ipt_entry* ipt, main_memory_entry* main_memory, int size, int pid, int window_size) {
  /* Create a set to store the containers of the time frame */
  int* working_set1 = initialize_set(window_size);
  int* working_set2 = initialize_set(window_size);

  /* Fill it */
  int total_items1, total_items2;
  fill_working_set(working_set1, active_pr1, window_size, &total_items1);
  fill_working_set(working_set2, active_pr2, window_size, &total_items2);
  /*If the size of the 2 sets is bigger than the total frames of the memory,
    we can not insert the page, thus we increase an error counter and return*/
  if (total_items1 + total_items2 > size) {
    ws_failures++;
    return EMPTY;
  }
  int* working_set;
  if (pid == 0) {
    working_set = working_set1;
  }
  else if (pid == 1) {
    working_set = working_set2;
  }
  else {
    printf("Something unexpected happened during page replacement. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  int pos = EMPTY;
  /* Scan the page table to find the tuples (pid, p#) that are not in the set */
  for (size_t i = 0; i < size; ++i) {
    if (ipt[i].pid == pid && in_set(working_set, i, window_size) == False) {
      /* "Remove" the page from the ipt and the main memory */
      ipt[i].pid = EMPTY;
      ipt[i].p_hash = EMPTY;
      /* Simulation of "writing to the disk", by setting the soon to be removed page
        as dirty */
      if (main_memory[i].modified == True) {
        dirty_pages++;
      }
      main_memory[i].curr_offset = EMPTY;
      main_memory[i].modified = 0;
      main_memory[i].time_signature = EMPTY;
      /* If it is the first time that we delete, we want the new page to be
        stored there */
      if (pos == EMPTY) {
        pos = i;
      }
    }
  }
  /* If we end up here, without sacrifising a page, something is definatelly
    wrond */
  if (pos == EMPTY) {
    printf("Something unexpected happened during page replacement. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  /* Free the set */
  free(working_set);

  /* Return the position of the page that was chosen to be sacrificed */
  return pos;
}
