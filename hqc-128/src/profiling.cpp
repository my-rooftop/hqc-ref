#include "profiling.h"
#include <cstdio>
#include <iomanip> // For setw() function

void time_analysis(Trace_time* trace_time) {
    // Exclude stack
    uint32_t total_time = trace_time->shake_prng_time + trace_time->seedexpander_init_time + trace_time->vect_set_random_fixed_weight_time + trace_time->vect_operation_time + trace_time->parsing_time + trace_time->code_encode_time + trace_time->vect_set_random_time + trace_time->code_decode_time + trace_time->shake256_512_ds_time;

    // Calculate percentages
    double shake_prng_percent = (trace_time->shake_prng_time * 100.0) / total_time;
    double seedexpander_init_percent = (trace_time->seedexpander_init_time * 100.0) / total_time;
    double vect_set_random_fixed_weight_percent = (trace_time->vect_set_random_fixed_weight_time * 100.0) / total_time;
    double vect_operation_percent = (trace_time->vect_operation_time * 100.0) / total_time;
    double parsing_percent = (trace_time->parsing_time * 100.0) / total_time;
    double code_encode_percent = (trace_time->code_encode_time * 100.0) / total_time;
    double vect_set_random_percent = (trace_time->vect_set_random_time * 100.0) / total_time;
    double code_decode_percent = (trace_time->code_decode_time * 100.0) / total_time;
    double shake256_512_ds_percent = (trace_time->shake256_512_ds_time * 100.0) / total_time;

    // Calculate time in milliseconds (ms)
    double shake_prng_ms = trace_time->shake_prng_time / static_cast<double>(trace_time->stack) / 1000.0;
    double seedexpander_init_ms = trace_time->seedexpander_init_time / static_cast<double>(trace_time->stack) / 1000.0;
    double vect_set_random_fixed_weight_ms = trace_time->vect_set_random_fixed_weight_time / static_cast<double>(trace_time->stack) / 1000.0;
    double vect_operation_ms = trace_time->vect_operation_time / static_cast<double>(trace_time->stack) / 1000.0;
    double parsing_ms = trace_time->parsing_time / static_cast<double>(trace_time->stack) / 1000.0;
    double code_encode_ms = trace_time->code_encode_time / static_cast<double>(trace_time->stack) / 1000.0;
    double vect_set_random_ms = trace_time->vect_set_random_time / static_cast<double>(trace_time->stack) / 1000.0;
    double code_decode_ms = trace_time->code_decode_time / static_cast<double>(trace_time->stack) / 1000.0;
    double shake256_512_ds_ms = trace_time->shake256_512_ds_time / static_cast<double>(trace_time->stack) / 1000.0;

    // Total time in milliseconds (ms)
    double total_ms = total_time / static_cast<double>(trace_time->stack) / 1000.0;

    // Print percentages and milliseconds in aligned columns
    printf("%-38s %-10s %-10s\n", "Function", "Time (ms)", "Percentage");
    printf("---------------------------------- ---------- ----------\n");
    printf("%-38s %.6fms (%.2f%%)\n", "shake_prng_time:", shake_prng_ms, shake_prng_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "seedexpander_init_time:", seedexpander_init_ms, seedexpander_init_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "vect_set_random_fixed_weight_time:", vect_set_random_fixed_weight_ms, vect_set_random_fixed_weight_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "vect_operation_time:", vect_operation_ms, vect_operation_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "parsing_time:", parsing_ms, parsing_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "code_encode_time:", code_encode_ms, code_encode_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "vect_set_random_time:", vect_set_random_ms, vect_set_random_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "code_decode_time:", code_decode_ms, code_decode_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "shake256_512_ds_time:", shake256_512_ds_ms, shake256_512_ds_percent);
    printf("\nTotal time: %.6fms (per stack)\n", total_ms);
    printf("\n");
}
