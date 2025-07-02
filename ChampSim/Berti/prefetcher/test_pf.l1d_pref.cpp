#include "cache.h"

#define TRANSITION_TABLE_SIZE 1024

struct transition_entry {
    uint64_t next_block;
    bool valid;
};

transition_entry transition_table[TRANSITION_TABLE_SIZE];

uint64_t last_miss_block = 0;

void CACHE::l1d_prefetcher_initialize() {
    cout << "L1D Transition-Based Prefetcher Initialized" << endl;
    for (int i = 0; i < TRANSITION_TABLE_SIZE; ++i) {
        transition_table[i].valid = false;
    }
}

// Main prefetcher logic
void CACHE::l1d_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint8_t critical_ip_flag) {
    // Only operate on misses
    if (cache_hit) {
        return;
    }

    // Get block-aligned address
    uint64_t current_block = addr >> LOG2_BLOCK_SIZE;

    if (last_miss_block != 0) {
        uint32_t index = last_miss_block % TRANSITION_TABLE_SIZE;

        
        if (transition_table[index].valid) {
            uint64_t prefetch_block_addr = transition_table[index].next_block << LOG2_BLOCK_SIZE;
            prefetch_line(ip, addr, prefetch_block_addr, FILL_L1, 0);
        }

       
        transition_table[index].next_block = current_block;
        transition_table[index].valid = true;
    }

    
    last_miss_block = current_block;
}


void CACHE::l1d_prefetcher_cache_fill(uint64_t v_addr, uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t v_evicted_addr, uint64_t evicted_addr, uint32_t metadata_in) {
}
