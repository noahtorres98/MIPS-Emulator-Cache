#ifndef __STATS_H
#define __STATS_H
#include <iostream>
#include "Debug.h"
using namespace std;

enum PIPESTAGE { IF1 = 0, IF2 = 1, ID = 2, EXE1 = 3, EXE2 = 4, MEM1 = 5, 
                 MEM2 = 6, WB = 7, PIPESTAGES = 8 };

class Stats {
  private:
    long long cycles;
    int flushes;
    int bubbles;
    int stalls;

    int memops;
    int branches;
    int taken;

    int resultReg[PIPESTAGES];
    int resultStage[PIPESTAGES];
    
    int rawCount;
    int rawEXE1;
    int rawEXE2;
    int rawMEM1;
    int rawMEM2;
    
    

  public:
    Stats();

    void clock();

    void flush(int count);
    
    void stall(int n);

    void registerSrc(int r, int valid);
    void registerDest(int r, int need);


    void countMemOp() { memops++; }
    void countBranch() { branches++; }
    void countTaken() { taken++; }

    // getters
    long long getCycles() { return cycles; }
    int getFlushes() { return flushes; }
    int getBubbles() { return bubbles; }
    int getStalls() { return stalls; }
    int getMemOps() { return memops; }
    int getBranches() { return branches; }
    int getTaken() { return taken; }
    int getRaw() { return rawCount; }
    int getRawEXE1() { return rawEXE1; }
    int getRawEXE2() { return rawEXE2; }
    int getRawMEM1() { return rawMEM1; }
    int getRawMEM2() { return rawMEM2; }
  private:
    void bubble();
};

#endif
