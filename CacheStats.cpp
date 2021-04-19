/******************************
 * Submitted by: Noah Torres; nst23
 * CS 3339 - Fall 2019, Texas State University
 * Project 5 Data Cache
 * Copyright 2019, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "CacheStats.h"
#include "Stats.h"
//#include "CPU.h"

using namespace std;

CacheStats::CacheStats() {
    cout << "Cache Config: ";
    if(!CACHE_EN) {
        cout << "cache disabled" << endl;
    } else {
        cout << (SETS * WAYS * BLOCKSIZE) << " B (";
        cout << BLOCKSIZE << " bytes/block, " << SETS << " sets, " << WAYS << " ways)" << endl;
        cout << "  Latencies: Lookup = " << LOOKUP_LATENCY << " cycles, ";
        cout << "Read = " << READ_LATENCY << " cycles, ";
        cout << "Write = " << WRITE_LATENCY << " cycles" << endl;
    }
    
    loads = 0;
    stores = 0;
    load_misses = 0;
    store_misses = 0;
    writebacks = 0;
    
    /* TODO: your code here */
    
    for (int i = 0; i < SETS; i++){
        roundRobin[i] = 0;
        for (int j = 0; j < WAYS; j++){
            tag[j][i] = -1;
            valid[j][i] = false;
            modified[j][i] = false;
        }
    }
}

int CacheStats::access(uint32_t addr, ACCESS_TYPE type) {
    
    if(!CACHE_EN) { // cache disabled
        return (type == LOAD) ? READ_LATENCY : WRITE_LATENCY;
    }
    
    if (type == STORE){
        stores++;
    }
    else {
        loads++;
    }
    
    int index = ((addr >> 5) & 0x7);
    int tempTag = (addr >> 8);
    int roundRobinVal = roundRobin[index];
    
    int returnVal = 0;
    
    // loops through ways, j = ways
    for (int j = 0; j < WAYS; j++){
        // if valid and hit
        if (tag[j][index] == tempTag && valid[j][index] == true) {
            if (type == STORE){
                modified[j][index] = true;
            }
            returnVal += LOOKUP_LATENCY;
            return returnVal;
        }
    }
    //miss
    if (type == STORE){
        store_misses++;
    }
    else{
        load_misses++;
    }
    //writeback
    if (modified[roundRobinVal][index] == true){ // old bloack is dirty
        returnVal += WRITE_LATENCY;
        writebacks++;
        modified[roundRobinVal][index] = false;
    }
    if (type == STORE){
        modified[roundRobinVal][index] = true;
    }
    //allocate
    tag[roundRobinVal][index] = tempTag; // reading new block
    valid[roundRobinVal][index] = true; // set valid to true
    roundRobin[index] = (roundRobin[index] + 1) % 4; //set up round robin
    returnVal += READ_LATENCY;
    
    return returnVal;
}


/* TODO: your code here */


void CacheStats::printFinalStats() {
    /* TODO: your code here (don't forget to drain the cache of writebacks) */
    
    int accesses = loads + stores;
    int misses = load_misses + store_misses;
    
    for(int i = 0; i < SETS; i++){
        for (int j = 0; j < WAYS; j++) {
            if (modified[j][i] == true){
                writebacks++;
            }
        }
    }
    
    cout << "Accesses: " << accesses << endl;
    cout << "  Loads: " << loads << endl;
    cout << "  Stores: " << stores << endl;
    cout << "Misses: " << misses << endl;
    cout << "  Load misses: " << load_misses << endl;
    cout << "  Store misses: " << store_misses << endl;
    cout << "Writebacks: " << writebacks << endl;
    cout << "Hit Ratio: " << fixed << setprecision(1) << 100.0 * (accesses - misses) / accesses;
    cout << "%" << endl;
}
