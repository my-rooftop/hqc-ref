/**
 * @file hqc.cpp
 * @brief Implementation of hqc.h
 */

#include "hqc.h"
#include "gf2x.h"
#include "parameters.h"
#include "parsing.h"
#include "shake_prng.h"
#include "code.h"
#include "vector.h"
#include "profiling.h"
#include <stdint.h>
#include <time.h>
#include <cstdint>
#ifdef VERBOSE
#include <stdio.h>
#endif


/**
 * @brief Keygen of the HQC_PKE IND_CPA scheme
 *
 * The public key is composed of the syndrome <b>s</b> as well as the <b>seed</b> used to generate the vector <b>h</b>.
 *
 * The secret key is composed of the <b>seed</b> used to generate vectors <b>x</b> and  <b>y</b>.
 * As a technicality, the public key is appended to the secret key in order to respect NIST API.
 *
 * @param[out] pk String containing the public key
 * @param[out] sk String containing the secret key
 */
void hqc_pke_keygen(unsigned char* pk, unsigned char* sk, Trace_time *keygen_time) {
    seedexpander_state sk_seedexpander;
    seedexpander_state pk_seedexpander;
    uint8_t sk_seed[SEED_BYTES] = {0};
    uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
    uint8_t pk_seed[SEED_BYTES] = {0};
    uint64_t x[VEC_N_SIZE_64] = {0};
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint64_t h[VEC_N_SIZE_64] = {0};
    uint64_t s[VEC_N_SIZE_64] = {0};
    clock_t start, end;

    // Create seed_expanders for public key and secret key
    // shake 알고리즘, 해시 함수 같은건데 NIST에 의해 SHA-3의 일부로 채택된것임, SHA-3 알고리즘의 변형
    keygen_time->stack += 1;
    
    start = clock();
    shake_prng(sk_seed, SEED_BYTES); // sk_seed를 squeeze함, 출력데이터를 생성한 상태
    shake_prng(sigma, VEC_K_SIZE_BYTES);
    shake_prng(pk_seed, SEED_BYTES); // pk_seed를 squeeze
    end = clock();
    keygen_time->shake_prng_time += ((uint32_t)(end - start));
    
    start = clock();
    seedexpander_init(&sk_seedexpander, sk_seed, SEED_BYTES); //sk_seed를 absorb함
    seedexpander_init(&pk_seedexpander, pk_seed, SEED_BYTES); //pk_seed를 absorb함
    end = clock();
    keygen_time->seedexpander_init_time += ((uint32_t)(end - start));

    // Compute secret key 
    start = clock();
    vect_set_random_fixed_weight(&sk_seedexpander, x, PARAM_OMEGA); //hamming weight로 x, y생성
    vect_set_random_fixed_weight(&sk_seedexpander, y, PARAM_OMEGA); //x, y는 secret key에 해당됨
    end = clock();
    keygen_time->vect_set_random_fixed_weight_time += ((uint32_t)(end - start));

    // Compute public key
    start = clock();
    vect_set_random(&pk_seedexpander, h); //h 벡터 생성
    end = clock();
    keygen_time->vect_set_random_time += ((uint32_t)(end - start));

    start = clock();
    vect_mul(s, y, h); 
    vect_add(s, x, s, VEC_N_SIZE_64); // syndrome 생성
    end = clock();
    keygen_time->vect_operation_time += ((uint32_t)(end - start));

    // Parse keys to string
    start = clock();
    hqc_public_key_to_string(pk, pk_seed, s); //syndrome도 pk니까
    hqc_secret_key_to_string(sk, sk_seed, sigma, pk);
    end = clock();
    keygen_time->parsing_time += ((uint32_t)(end - start));

    #ifdef VERBOSE
        printf("\n\nsk_seed: "); for(int i = 0 ; i < SEED_BYTES ; ++i) printf("%02x", sk_seed[i]);
        printf("\n\nsigma: "); for(int i = 0 ; i < VEC_K_SIZE_BYTES ; ++i) printf("%02x", sigma[i]);
        printf("\n\nx: "); vect_print(x, VEC_N_SIZE_BYTES);
        printf("\n\ny: "); vect_print(y, VEC_N_SIZE_BYTES);

        printf("\n\npk_seed: "); for(int i = 0 ; i < SEED_BYTES ; ++i) printf("%02x", pk_seed[i]);
        printf("\n\nh: "); vect_print(h, VEC_N_SIZE_BYTES);
        printf("\n\ns: "); vect_print(s, VEC_N_SIZE_BYTES);

        printf("\n\nsk: "); for(int i = 0 ; i < SECRET_KEY_BYTES ; ++i) printf("%02x", sk[i]);
        printf("\n\npk: "); for(int i = 0 ; i < PUBLIC_KEY_BYTES ; ++i) printf("%02x", pk[i]);
    #endif
}



/**
 * @brief Encryption of the HQC_PKE IND_CPA scheme
 *
 * The cihertext is composed of vectors <b>u</b> and <b>v</b>.
 *
 * @param[out] u Vector u (first part of the ciphertext)
 * @param[out] v Vector v (second part of the ciphertext)
 * @param[in] m Vector representing the message to encrypt
 * @param[in] theta Seed used to derive randomness required for encryption
 * @param[in] pk String containing the public key
 */
void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk, Trace_time* common_time) {
    seedexpander_state seedexpander;
    uint64_t h[VEC_N_SIZE_64] = {0};
    uint64_t s[VEC_N_SIZE_64] = {0};
    uint64_t r1[VEC_N_SIZE_64] = {0};
    uint64_t r2[VEC_N_SIZE_64] = {0};
    uint64_t e[VEC_N_SIZE_64] = {0};
    uint64_t tmp1[VEC_N_SIZE_64] = {0};
    uint64_t tmp2[VEC_N_SIZE_64] = {0};
    clock_t start, end;
    // Create seed_expander from theta
    start = clock();
    seedexpander_init(&seedexpander, theta, SEED_BYTES); //Shake 256 처리
    end = clock();
    common_time->seedexpander_init_time += ((uint32_t)(end - start));

    // Retrieve h and s from public key
    hqc_public_key_from_string(h, s, pk, common_time); //h, s 추출?

    // Generate r1, r2 and e
    start = clock();
    vect_set_random_fixed_weight(&seedexpander, r1, PARAM_OMEGA_R);
    vect_set_random_fixed_weight(&seedexpander, r2, PARAM_OMEGA_R);
    vect_set_random_fixed_weight(&seedexpander, e, PARAM_OMEGA_E);
    end = clock();
    common_time->vect_set_random_fixed_weight_time += ((uint32_t)(end - start));
    //r1, r2, e 벡터생성

    // Compute u = r1 + r2.h
    start = clock();
    vect_mul(u, r2, h);
    vect_add(u, r1, u, VEC_N_SIZE_64); //u 연산
    end = clock();
    common_time->vect_operation_time += ((uint32_t)(end - start));

    // Compute v = m.G by encoding the message
    start = clock();
    code_encode(v, m); //rs-rm encoding
    end = clock();
    common_time->code_encode_time += ((uint32_t)(end - start));

    start = clock();
    vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);

    // Compute v = m.G + s.r2 + e
    vect_mul(tmp2, r2, s);
    vect_add(tmp2, e, tmp2, VEC_N_SIZE_64);
    vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
    vect_resize(v, PARAM_N1N2, tmp2, PARAM_N);
    end = clock();
    common_time->vect_operation_time += ((uint32_t)(end - start));

    #ifdef VERBOSE
        printf("\n\nh: "); vect_print(h, VEC_N_SIZE_BYTES);
        printf("\n\ns: "); vect_print(s, VEC_N_SIZE_BYTES);
        printf("\n\nr1: "); vect_print(r1, VEC_N_SIZE_BYTES);
        printf("\n\nr2: "); vect_print(r2, VEC_N_SIZE_BYTES);
        printf("\n\ne: "); vect_print(e, VEC_N_SIZE_BYTES);
        printf("\n\ntmp2: "); vect_print(tmp2, VEC_N_SIZE_BYTES);

        printf("\n\nu: "); vect_print(u, VEC_N_SIZE_BYTES);
        printf("\n\nv: "); vect_print(v, VEC_N1N2_SIZE_BYTES);
    #endif
}


void hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint64_t *m, unsigned char *theta, const unsigned char *pk) {
    seedexpander_state seedexpander;
    uint64_t h[VEC_N_SIZE_64] = {0};
    uint64_t s[VEC_N_SIZE_64] = {0};
    uint64_t r1[VEC_N_SIZE_64] = {0};
    uint64_t r2[VEC_N_SIZE_64] = {0};
    uint64_t e[VEC_N_SIZE_64] = {0};
    uint64_t tmp1[VEC_N_SIZE_64] = {0};
    uint64_t tmp2[VEC_N_SIZE_64] = {0};

    // Create seed_expander from theta
    seedexpander_init(&seedexpander, theta, SEED_BYTES); //Shake 256 처리

    // Retrieve h and s from public key
    hqc_public_key_from_string(h, s, pk); //h, s 추출?

    // Generate r1, r2 and e
    vect_set_random_fixed_weight(&seedexpander, r1, PARAM_OMEGA_R);
    vect_set_random_fixed_weight(&seedexpander, r2, PARAM_OMEGA_R);
    vect_set_random_fixed_weight(&seedexpander, e, PARAM_OMEGA_E);
    //r1, r2, e 벡터생성

    // Compute u = r1 + r2.h
    vect_mul(u, r2, h);
    vect_add(u, r1, u, VEC_N_SIZE_64); //u 연산

    // Compute v = m.G by encoding the message
    code_encode(v, m); //rs-rm encoding
    vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);

    // Compute v = m.G + s.r2 + e
    vect_mul(tmp2, r2, s);
    vect_add(tmp2, e, tmp2, VEC_N_SIZE_64);
    vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
    vect_resize(v, PARAM_N1N2, tmp2, PARAM_N);

    #ifdef VERBOSE
        printf("\n\nh: "); vect_print(h, VEC_N_SIZE_BYTES);
        printf("\n\ns: "); vect_print(s, VEC_N_SIZE_BYTES);
        printf("\n\nr1: "); vect_print(r1, VEC_N_SIZE_BYTES);
        printf("\n\nr2: "); vect_print(r2, VEC_N_SIZE_BYTES);
        printf("\n\ne: "); vect_print(e, VEC_N_SIZE_BYTES);
        printf("\n\ntmp2: "); vect_print(tmp2, VEC_N_SIZE_BYTES);

        printf("\n\nu: "); vect_print(u, VEC_N_SIZE_BYTES);
        printf("\n\nv: "); vect_print(v, VEC_N1N2_SIZE_BYTES);
    #endif
}


/**
 * @brief Decryption of the HQC_PKE IND_CPA scheme
 *
 * @param[out] m Vector representing the decrypted message
 * @param[in] u Vector u (first part of the ciphertext)
 * @param[in] v Vector v (second part of the ciphertext)
 * @param[in] sk String containing the secret key
 * @returns 0 
 */
uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk, Trace_time* decap_time) {
    uint64_t x[VEC_N_SIZE_64] = {0};
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
    uint64_t tmp1[VEC_N_SIZE_64] = {0};
    uint64_t tmp2[VEC_N_SIZE_64] = {0};
    clock_t start, end;

    // Retrieve x, y, pk from secret key
    hqc_secret_key_from_string(x, y, sigma, pk, sk, decap_time);
    //두번의 벡터 생성과 시드 작업

    // Compute v - u.y
    start = clock();
    vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);
    vect_mul(tmp2, y, u);
    vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
    end = clock();
    decap_time->vect_operation_time += ((uint32_t)(end - start));
    // decap_time 
    // 사이즈 변경 및 계산

    #ifdef VERBOSE
        printf("\n\nu: "); vect_print(u, VEC_N_SIZE_BYTES);
        printf("\n\nv: "); vect_print(v, VEC_N1N2_SIZE_BYTES);
        printf("\n\ny: "); vect_print(y, VEC_N_SIZE_BYTES);
        printf("\n\nv - u.y: "); vect_print(tmp2, VEC_N_SIZE_BYTES);
    #endif

    // Compute m by decoding v - u.y
    start = clock();
    code_decode(m, tmp2);
    end = clock();
    decap_time->code_decode_time += ((uint32_t)(end - start));
    //rm-rs decoding 연산
    
    return 0;
}

uint8_t hqc_pke_decrypt(uint64_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const uint8_t *sk) {
    uint64_t x[VEC_N_SIZE_64] = {0};
    uint64_t y[VEC_N_SIZE_64] = {0};
    uint8_t pk[PUBLIC_KEY_BYTES] = {0};
    uint64_t tmp1[VEC_N_SIZE_64] = {0};
    uint64_t tmp2[VEC_N_SIZE_64] = {0};

    // Retrieve x, y, pk from secret key
    hqc_secret_key_from_string(x, y, sigma, pk, sk);
    //두번의 벡터 생성과 시드 작업

    // Compute v - u.y
    vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);
    vect_mul(tmp2, y, u);
    vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
    // 사이즈 변경 및 계산

    #ifdef VERBOSE
        printf("\n\nu: "); vect_print(u, VEC_N_SIZE_BYTES);
        printf("\n\nv: "); vect_print(v, VEC_N1N2_SIZE_BYTES);
        printf("\n\ny: "); vect_print(y, VEC_N_SIZE_BYTES);
        printf("\n\nv - u.y: "); vect_print(tmp2, VEC_N_SIZE_BYTES);
    #endif

    // Compute m by decoding v - u.y
    code_decode(m, tmp2);
    //rm-rs decoding 연산
    
    return 0;
}
