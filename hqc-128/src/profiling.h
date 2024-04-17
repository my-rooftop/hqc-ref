#ifndef PROFILING_H
#define PROFILING_H

#include <cstdint>
#include <stdio.h>

struct Trace_time {
  uint32_t stack = 0;
  uint32_t shake_prng_time = 0;
  uint32_t seedexpander_init_time = 0;
  uint32_t vect_set_random_fixed_weight_time = 0;
  uint32_t vect_operation_time = 0;
  uint32_t parsing_time = 0;
  uint32_t code_encode_time = 0;
  uint32_t vect_set_random_time = 0;
  uint32_t code_decode_time = 0;
  uint32_t shake256_512_ds_time = 0;
};

void time_analysis(Trace_time* trace_time);


#endif