#ifndef CODE_H
#define CODE_H

/**
 * @file code.h
 * @brief Header file of code.c
 */

#include "parameters.h"
#include "profiling.h"
#include <stddef.h>
#include <stdint.h>

void code_encode(uint64_t *codeword, const uint64_t *message, struct Trace_time* trace_time);
void code_decode(uint64_t *message, const uint64_t *vector, struct Trace_time* trace_time);

#endif
