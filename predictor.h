/*
 * University of California, San Diego
 *    
 *    CSE141L Summer 2017
 *
 */

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <stdint.h>
#include <stdlib.h>

//------------------------------------//
//      Global Predictor Defines      //
//------------------------------------//
#define NOTTAKEN  0
#define TAKEN     1

// The Different Predictor Types
#define ALLTAKEN      0
#define ALLNOTTAKEN      1
#define TWOBIT		2
#define CUSTOM     3 

// Definitions for 2-bit counters
#define SN  0			// predict NT, strong not taken
#define WN  1			// predict NT, weak not taken
#define WT  2			// predict T, weak taken
#define ST  3			// predict T, strong taken

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//
extern int bpType;       // Branch Prediction Type
extern int verbose;

//------------------------------------//
//    Predictor Function Prototypes   //
//------------------------------------//

class Predictor{
	public:
		int twobitstate;

		// Initialize the predictor
		//
		Predictor();
		
		// Make a prediction for conditional branch instruction at PC 'pc'
		// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
		// indicates a prediction of not taken
		//
		uint8_t makePrediction(uint32_t pc);
		
		
		// Train the predictor the last executed branch at PC 'pc' and with
		// outcome 'outcome' (true indicates that the branch was taken, false
		// indicates that the branch was not taken)
		//
		void trainPredictor(uint32_t pc, uint8_t outcome);

		// void setState(int state);

		// int getState();
};

#endif
