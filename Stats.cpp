/******************************
 * Submitted by: Noah Torres; nst23
 * CS 3339 - Fall 2019, Texas State University
 * Project 3 Pipelining
 * Copyright 2019, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/

#include "Stats.h"

Stats::Stats() {
    cycles = PIPESTAGES - 1; // pipeline startup cost
    flushes = 0;
    bubbles = 0;
    stalls = 0;
    
    memops = 0;
    branches = 0;
    taken = 0;
    
    rawCount = 0;
    rawEXE1 = 0;
    rawEXE2 = 0;
    rawMEM1 = 0;
    rawMEM2 = 0;
    
    for (int i = IF1; i < PIPESTAGES; i++) {
        resultReg[i] = -1;
    }
    
    for (int i = IF1; i < PIPESTAGES; i++) {
        resultStage[i] = 0;
    }
}

void Stats::clock() {
    cycles++;
    
    // advance all pipeline flip-flops
    for(int i = WB; i > IF1; i--) {
        resultReg[i] = resultReg[i-1];
        resultStage[i] = resultStage[i-1];
    }
    // inject a no-op into IF1
    resultReg[IF1] = -1;
}

void Stats::registerSrc(int r, int need) {
    if (r == 0){
        return;
    }
    for (int i = EXE1; i < WB; i++) {
        if (r == resultReg[i]) {
            rawCount++;
            if (i == EXE1){rawEXE1++;}
            if (i == EXE2){rawEXE2++;}
            if (i == MEM1){rawMEM1++;}
            if (i == MEM2){rawMEM2++;}
            int becomesAvail = resultStage[i];
            int clocksNeeded = becomesAvail - i;
            int clocksUntilNeed = need - ID;
            int numBubbles = clocksNeeded - clocksUntilNeed;
            for (int j = 0; j < numBubbles; j++){
                bubble();
            }
            break;
        }
        
    }
}

void Stats::registerDest(int r, int valid) {
    resultReg[ID] = r;
    resultStage[ID] = valid;
}

void Stats::flush(int count) { // count == how many ops to flush
    for (int i = 0; i < count; i++){
        flushes++;
        cycles++;
        
        // advance all pipeline flip-flops
        for(int i = WB; i > IF1; i--) {
            resultReg[i] = resultReg[i-1];
            resultStage[i] = resultStage[i-1];
        }
        
        // inject a no-op into IF1
        resultReg[IF1] = -1;
    }
    
}

void Stats::bubble() {
    bubbles++;
    cycles++;
    
    // advance all pipeline flip-flops
    for(int i = WB; i > EXE1; i--) {
        resultReg[i] = resultReg[i-1];
        resultStage[i] = resultStage[i-1];
    }
    
    // inject a no-op into IF1
    resultReg[EXE1] = -1;
    
}

void Stats::stall(int n) {
    cycles = cycles + n;
    stalls = stalls + n;
}
