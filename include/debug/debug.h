/**
 * @file debug.h
 * @author Jacob LuVisi
 * @brief Basic debug and performance metric functions. This header file does not have a seperate class all possible functions are inlined.
 * @version 0.1
 * @date 2021-07-26
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <studio-libs/tune_studio.h>
#ifndef debug_h
#define debug_h
#if PERF_METRICS == true
 // measure memory usage.
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
#endif  // __arm__

inline unsigned int freeMemory() {
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

// C++ for Arduino
// What is heap fragmentation?
// https://cpp4arduino.com/

// This source file captures the platform dependent code.
// This version was tested with the AVR Core version 1.6.22

// This code is freely inspired from https://github.com/McNeight/MemoryFree

// This heap allocator defines this structure to keep track of free blocks.
struct block_t {
    size_t sz;
    struct block_t* nx;
};

// NOTE. The following extern variables are defined in malloc.c in avr-stdlib

// A pointer to the first block
extern struct block_t* __flp;

// A pointer to the end of the heap, initialized at first malloc()
extern char* __brkval;

// A pointer to the beginning of the heap
extern char* __malloc_heap_start;

inline static size_t getBlockSize(struct block_t* block) {
    return block->sz + 2;
}

inline static size_t getUnusedBytes() {
    char foo;
    if (__brkval) {
        return size_t(&foo - __brkval);
    }
    else {
        return size_t(&foo - __malloc_heap_start);
    }
}

inline size_t getTotalAvailableMemory() {
    size_t sum = getUnusedBytes();
    for (struct block_t* block = __flp; block; block = block->nx) {
        sum += getBlockSize(block);
    }
    return sum;
}

inline size_t getLargestAvailableBlock() {
    size_t largest = getUnusedBytes();
    for (struct block_t* block = __flp; block; block = block->nx) {
        size_t size = getBlockSize(block);
        if (size > largest) {
            largest = size;
        }
    }
    return largest;
}

inline float getFragmentation() {
    return 100 - getLargestAvailableBlock() * 100.0 / getTotalAvailableMemory();
}
#endif
#endif