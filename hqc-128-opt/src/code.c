/**
 * @file code.c
 * @brief Implementation of concatenated code
 */

#include "code.h"
#include "reed_muller.h"
#include "reed_solomon.h"
#include "parameters.h"
#include <stdint.h>
#include <string.h>
#include <time.h>
#ifdef VERBOSE
#include <stdio.h>
#include "vector.h"
#endif


/**
 *
 * @brief Encoding the message m to a code word em using the concatenated code
 *
 * First we encode the message using the Reed-Solomon code, then with the duplicated Reed-Muller code we obtain
 * a concatenated code word.
 *
 * @param[out] em Pointer to an array that is the tensor code word
 * @param[in] m Pointer to an array that is the message
 */
void code_encode(uint64_t *em, const uint64_t *m, struct Trace_time* trace_time) {
    uint64_t tmp[VEC_N1_SIZE_64] = {0};
    clock_t start, end;

    start = clock();
    reed_solomon_encode(tmp, m);
    end = clock();
    trace_time->rs_encode_time += ((uint32_t)(end - start));
    
    start = clock();
    reed_muller_encode(em, tmp);
    end = clock();
    trace_time->rm_encode_time += ((uint32_t)(end - start));

    #ifdef VERBOSE
        printf("\n\nReed-Solomon code word: "); vect_print(tmp, VEC_N1_SIZE_BYTES);
        printf("\n\nConcatenated code word: "); vect_print(em, VEC_N1N2_SIZE_BYTES);
    #endif
}



/**
 * @brief Decoding the code word em to a message m using the concatenated code
 *
 * @param[out] m Pointer to an array that is the message
 * @param[in] em Pointer to an array that is the code word
 */
void code_decode(uint64_t *m, const uint64_t *em, struct Trace_time* trace_time) {
    uint64_t tmp[VEC_N1_SIZE_64] = {0};
    clock_t start, end;

    start = clock();
    reed_muller_decode(tmp, em);
    end = clock();
    trace_time->rm_decode_time += ((uint32_t)(end - start));

    start = clock();
    reed_solomon_decode(m, tmp, trace_time);
    end = clock();
    trace_time->rs_decode_time += ((uint32_t)(end - start));


    #ifdef VERBOSE
        printf("\n\nReed-Muller decoding result (the input for the Reed-Solomon decoding algorithm): "); vect_print(tmp, VEC_N1_SIZE_BYTES);
    #endif
}
