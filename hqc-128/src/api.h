/**
 * @file api.h
 * @brief NIST KEM API used by the HQC_KEM IND-CCA2 scheme
 */

#ifndef API_H
#define API_H
#include "profiling.h"
#define CRYPTO_ALGNAME                      "HQC-128"

#define CRYPTO_SECRETKEYBYTES               2305
#define CRYPTO_PUBLICKEYBYTES               2249
#define CRYPTO_BYTES                        64
#define CRYPTO_CIPHERTEXTBYTES              4433

// As a technicality, the public key is appended to the secret key in order to respect the NIST API.
// Without this constraint, CRYPTO_SECRETKEYBYTES would be defined as 32

int crypto_kem_keypair(unsigned char* pk, unsigned char* sk, Trace_time* keygen_time);
int crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk, Trace_time* encap_time);
int crypto_kem_enc(unsigned char* ct, unsigned char* ss, const unsigned char* pk);
int crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk, Trace_time* decap_time);
int crypto_kem_dec(unsigned char* ss, const unsigned char* ct, const unsigned char* sk);

#endif
