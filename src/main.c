#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/simulator.h"

int main(int argc, char const *argv[]) {
  /* Check for correct use of program */
  if (argc < 4 || argc > 5) {
    printf("Wrong arguments. Use as: [repl. algorithm n_frames set_length max_refs window_size]\n");
    exit(EXIT_FAILURE);
  }
  /* Catch the info of the arguments */
  int algorithm;
  if (strcmp(argv[1], "LRU") == 0)
    algorithm = LRU;
  else if (strcmp(argv[1], "WS") == 0)
    algorithm = WS;
  else {
    printf("Error: Wrong algorithm given. Choose one of LRU | WS. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  int n_frames = atoi(argv[2]);
  int set_length = atoi(argv[3]);
  int max_references = atoi(argv[4]);
  int window_size = 0;
  if (algorithm == WS)
    window_size = atoi(argv[5]);
  /* Run the simulator */
  simulator(algorithm, n_frames, set_length, max_references, window_size);

  exit(EXIT_SUCCESS);
}
