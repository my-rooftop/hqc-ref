#ifndef HQC_H
#define HQC_H

/**
 * @file hqc.h
 * @brief Functions of the HQC_PKE IND_CPA scheme
 */

#include <stdint.h>
#include "profiling.h"

void hqc_pke_keygen(unsigned char* pk, unsigned char* sk, Trace_time *keygen_time);
void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk);
void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk, Trace_time* common_time);
uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk);
uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk, Trace_time* decap_time);

#endif