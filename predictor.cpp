/*
 * University of California, San Diego
 *    
 *    CSE141L Summer 2017
 *
 */

#include "predictor.h"


Predictor::Predictor() {
  twobitstate = WN;
}


uint8_t Predictor::makePrediction(uint32_t pc) {
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case ALLTAKEN:
      return TAKEN;
      break;
    case ALLNOTTAKEN:
      pc = pc + 4;
      return NOTTAKEN;
      break;
    // TWOBIT starts with WN (Weakly-Not-Taken)
    case TWOBIT:
      if( twobitstate == SN || twobitstate == WN ) {
        pc = pc + 4;
        return NOTTAKEN;
      } else {
        return TAKEN;
      }
      break;
    case CUSTOM:
    default:
      break;
  }

  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void Predictor::trainPredictor(uint32_t pc, uint8_t outcome) {
  //
  //TODO: Implement Predictor training
  //
  if ( bpType == ALLTAKEN || bpType == ALLNOTTAKEN ) { return; }
  else if ( bpType == TWOBIT ) {
    if( outcome == TAKEN ) {
      if( twobitstate == SN ) {
        twobitstate = WN;
      } else if ( twobitstate == WN ) {
        twobitstate = WT;
      } else {
        twobitstate = ST;
      }
    } else {
      if( twobitstate == WN ) {
        twobitstate = SN;
      } else if ( twobitstate == WT ) {
        twobitstate = WN;
      } else {
        twobitstate = WT;
      }
    }
  }
  else {
    //Custom Branch Predictor
  }
}