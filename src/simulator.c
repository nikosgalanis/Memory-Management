#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../headers/simulator.h"
#include "../headers/replacement_algorithms.h"


void simulator(int alg, int n_frames, int set_length, int max, int window_size) {

  ipt_entry* ipt = create_ipt(n_frames);
  /* Open the 2 files */
  FILE* process1, *process2;
  process1 = fopen("input/bzip.trace", "r");
  process2 = fopen("input/gcc.trace", "r");
  int addr; char operation;
  size_t loops = max / set_length;
  int pid;
  set_entry* temp_set = create_temp_set(set_length);
  main_memory_entry* main_memory = create_main_memory(n_frames);
  /* Initialize 2 queues that will store the time frame needed for the WS algo */
  active_pr1 = initialize_queue(window_size);
  active_pr2 = initialize_queue(window_size);

  for (size_t i = 0; i < 2 * loops; ++i) {

    /* Fill the temp set with the data from one of the files. If we are on an
        odd loop, we read from second file, lse from the first */
    if (i % 2 == 0) {
      pid = 0;
      fill_set(temp_set, process1, set_length);
    }
    else {
      pid = 1;
      fill_set(temp_set, process2, set_length);
    }
    /* Traverse the set of logical adresses and try to insert them in the ipt */
    for (size_t j = 0; j < set_length; ++j) {
      logical_adress* log_adress = create_logical_adress(temp_set[j].address);
      int result; int pos = EMPTY;
      /* Traverse the ipt to find empty space, or even better, the page itself */
      result = find_space_ipt(ipt, n_frames, pid, log_adress->p_hash , &pos);
      /* If we find the page, then just adhust the entry in the main memory */
      if (result == FOUND_PAGE) {
        /* Nothing to change on the ipt, just update the succesfull counter */
        succesfull_searches++;
      }
      /* If we find an empty space, then we have a page fault, but we have where
        to insert the new page */
      else if (result == FOUND_EMPTY_SPACE) {
        page_faults++;
        ipt[pos].pid = pid;
        ipt[pos].p_hash = log_adress->p_hash;
        //TODO change stuff in main memory
      }
      /* Worst case scenario: ipt full and page not found. So we have a page
        fault, and we must decide which page to sacrifice, by running the algorithm
        given as an argument to the simulator */
      else if (result == FAILURE) {
        pos = find_page_to_sacrifise(alg, ipt, main_memory, n_frames, pid, window_size);

        if (pos != EMPTY) {
          page_faults++;
          ipt[pos].pid = pid;
          ipt[pos].p_hash = log_adress->p_hash;
        }
      }
      /* If we are running WS, we want the new page in to the correct time frame */
      if (pid == 0 && alg == WS && pos != EMPTY) {
        insert(active_pr1, pos);
      }
      else if (pid == 1 && alg == WS && pos != EMPTY) {
        insert(active_pr2, pos);
      }
      if (pos != EMPTY) {
        /* Change the page info in the main memory, by accessing the correct
        position found(or created) in the ipt */
        main_memory[pos].curr_offset = log_adress->offset;
        main_memory[pos].time_signature = time;
      }
      /* If we want to write, store that info in order to set the page dirty
        later. Also, update the corrensponting counter */
      if (temp_set[j].operation == 'W' && pos != EMPTY) {
        main_memory[pos].modified = True;
        writes++;
      }
      else {
        reads++;
      }
      /* Increase the virtual time by 1 in each loop*/
      time++;
      free(log_adress);
    }
  }

  /* Print the results of the simulation, and create log files */
  printf("-----------Simulator ended!-----------\n");
  printf("Operated on a virtual memory of %d frames, with frame size 4096\n", n_frames);
  printf("Results:\n");
  printf("Total traces examined for each file were: %d\n", max);
  printf("Total page faults: %d\n",page_faults);
  printf("Page Fault Rate (PFR): %f\n", (float)page_faults / time);
  printf("Reads %d, Writes %d\n",reads, writes);
  printf("We've set %d pages as dirty\n",dirty_pages);
  if (alg == WS) {
    printf("A total of %d pages were not inserted, because of the violation of the WS size rule.\n", ws_failures);
  }

  char answer;
  printf("Do you want to create log files containing the final picture of the IPT and the main memory? (Y/N)\n");
  scanf("%c",&answer);
  if (answer == 'Y' || answer == 'y') {
    mkdir("output", ACCESSPERMS);
    FILE* ipt_pic, *mem_pic;
    ipt_pic = fopen("output/IPT", "w+");
    mem_pic = fopen("output/MEM", "w+");
    fprintf(ipt_pic, "Final picture of the IPT.\n\np_id | p#\n\n");
    fprintf(mem_pic, "Final picture of the main memory.\n\noffset | M | time_signature\n\n");

    for (size_t i = 0; i < n_frames; ++i) {
      fprintf(ipt_pic, "%2d     %X\n",ipt[i].pid, ipt[i].p_hash);
      fprintf(mem_pic, "%3X      %2d   %d\n", main_memory[i].curr_offset, main_memory[i].modified, main_memory[i].time_signature);
    }

    if (fclose(ipt_pic) == EOF || fclose(mem_pic) == EOF) {
      printf("Error while closing files. Exiting...\n");
      exit(EXIT_FAILURE);
    }
  }
  /* Close the 2 trace files, and catch a possible error */
  if (fclose(process1) == EOF || fclose(process2) == EOF) {
    printf("Error while closing files. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  /* Free the structs that we allocated for the simulation */
  free(ipt);
  free(main_memory);
  free(temp_set);

  return;
}
