#ifndef CODE_H
#define CODE_H

/**
 * @file code.h
 * @brief Header file of code.cpp
 */

#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void code_encode(uint64_t *codeword, const uint64_t *message, Trace_time* common_time);
void code_encode(uint64_t *codeword, const uint64_t *message);
void code_decode(uint64_t *message, const uint64_t *vector, Trace_time* common_time);
void code_decode(uint64_t *message, const uint64_t *vector);

#endif
