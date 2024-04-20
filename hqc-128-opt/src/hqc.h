#ifndef HQC_H
#define HQC_H

/**
 * @file hqc.h
 * @brief Functions of the HQC_PKE IND_CPA scheme
 */

#include <stdint.h>
#include <immintrin.h>
#include "profiling.h"

void hqc_pke_keygen(unsigned char* pk, unsigned char* sk, struct Trace_time *trace_time);
void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk, struct Trace_time *trace_time);
uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const __m256i *u_256, const uint64_t *v, const uint8_t *sk, struct Trace_time *trace_time);

#endif