#ifndef PROFILING_H
#define PROFILING_H

#include <stdint.h>
#include <stdio.h>

struct Trace_time {
  uint32_t stack;
  uint32_t shake_prng_time;
  uint32_t seedexpander_init_time;
  uint32_t vect_set_random_fixed_weight_time;
  uint32_t vect_operation_time;
  uint32_t parsing_time;
  uint32_t rs_encode_time;
  uint32_t rm_encode_time;
  uint32_t vect_set_random_time;
  uint32_t rs_decode_time;
  uint32_t rm_decode_time;
  uint32_t shake256_512_ds_time;
  //--------- rs_decode details ---------//
  uint32_t compute_syndromes_time;
  uint32_t compute_elp_time;
  uint32_t compute_roots_time;
  uint32_t compute_z_poly_time;
  uint32_t compute_error_values_time;
  uint32_t correct_errors_time;
};

void time_analysis(struct Trace_time* trace_time);
void rs_decode_detail_analysis(struct Trace_time* trace_time);
void initialize_trace_time(struct Trace_time *trace_time);


#endif