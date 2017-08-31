/*
 * University of California, San Diego
 *    
 *    CSE141L Summer 2017
 *
 */
 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "predictor.h"

FILE *stream;
char *buf = NULL;
size_t len = 0;
int bpType;
int verbose;

// Print out the Usage information to stderr
//
void
usage()
{
  fprintf(stderr,"Usage: predictor <options> [<trace>]\n");
  fprintf(stderr," Options:\n");
  fprintf(stderr," --help       Print this message\n");
  fprintf(stderr," --verbose    Print predictions on stdout\n");
  fprintf(stderr," --<type>     Branch prediction scheme:\n");
  fprintf(stderr,"    AT        All taken predictor\n"
                 "    ANT       All not taken predictor\n"
                 "    TWOBIT    Two bit dynamic predictor\n"
                 "    CUSTOM    Custom predictor\n");
}

// Process an option and update the predictor
// configuration variables accordingly
//
// Returns True if Successful
//
int
handle_option(char *arg)
{
  if (!strcmp(arg,"--AT")) {
    bpType = ALLTAKEN;
  } else if (!strcmp(arg,"--ANT")) {
    bpType = ALLNOTTAKEN;
  } else if (!strcmp(arg,"--CUSTOM")) {
    bpType = CUSTOM;
  } else if (!strcmp(arg,"--TWOBIT")) {
    bpType = TWOBIT;
  } else if (!strcmp(arg,"--verbose")) {
    verbose = 1;
  } else {
    return 0;
  }

  return 1;
}

// Reads a line from the input stream and extracts the
// PC and Outcome of a branch
//
// Returns True if Successful 
//
int
read_branch(uint32_t *pc, uint8_t *outcome)
{
  if (getline(&buf, &len, stream) == -1) {
    return 0;
  }

  uint32_t tmp;
  sscanf(buf,"0x%x %d\n",pc,&tmp);
  *outcome = tmp;

  return 1;
}

int
main(int argc, char *argv[])
{
  Predictor predictor;
  
  // Set defaults
  stream = stdin;
  bpType = ALLTAKEN;
  verbose = 0;

  // Process cmdline Arguments
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i],"--help")) {
      usage();
      exit(0);
    } else if (!strncmp(argv[i],"--",2)) {
      if (!handle_option(argv[i])) {
        printf("Unrecognized option %s\n", argv[i]);
        usage();
        exit(1);
      }
    } else {
      // Use as input file
      stream = fopen(argv[i], "r");
    }
  }


  uint32_t num_branches = 0;
  uint32_t mispredictions = 0;
  uint32_t pc = 0;
  uint8_t outcome = NOTTAKEN;

  // Reach each branch from the trace
  while (read_branch(&pc, &outcome)) {
    num_branches++;

    // Make a prediction and compare with actual outcome
    uint8_t prediction = predictor.makePrediction(pc);
    if (prediction != outcome) {
      mispredictions++;
    }
    if (verbose != 0) {
      printf ("%d\n", prediction);
    }

    /*int
      read_branch(uint32_t *pc, uint8_t *outcome)
      {
        if (getline(&buf, &len, stream) == -1) {
          return 0;
        }

        uint32_t tmp;
        sscanf(buf,"0x%x %d\n",pc,&tmp);
        *outcome = tmp;

        return 1;
      }*/

    // Train the predictor
    predictor.trainPredictor(pc, outcome);
  }

  // Print out the mispredict statistics
  printf("Branches:        %10d\n", num_branches);
  printf("Incorrect:       %10d\n", mispredictions);
  float mispredict_rate = 100*((float)mispredictions / (float)num_branches);
  printf("Misprediction Rate: %7.3f\n", mispredict_rate);

  // Cleanup
  fclose(stream);
  free(buf);

  return 0;
}
