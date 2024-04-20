#include "profiling.h"
#include <cstdio>
#include <iomanip> // For setw() function

void time_analysis(Trace_time* trace_time) {
    // Exclude stack, shake_prng_time, seedexpander_init_time, and parsing_time
    uint32_t total_time = trace_time->parsing_time + trace_time->seedexpander_init_time + trace_time->shake_prng_time + trace_time->vect_set_random_fixed_weight_time + trace_time->vect_operation_time +
                          trace_time->rs_encode_time + trace_time->rm_encode_time + trace_time->vect_set_random_time +
                          trace_time->rs_decode_time + trace_time->rm_decode_time + trace_time->shake256_512_ds_time;

    // Calculate percentages
    double vect_set_random_fixed_weight_percent = (trace_time->vect_set_random_fixed_weight_time * 100.0) / total_time;
    double vect_operation_percent = (trace_time->vect_operation_time * 100.0) / total_time;
    double rs_encode_percent = (trace_time->rs_encode_time * 100.0) / total_time;
    double rm_encode_percent = (trace_time->rm_encode_time * 100.0) / total_time;
    double vect_set_random_percent = (trace_time->vect_set_random_time * 100.0) / total_time;
    double rs_decode_percent = (trace_time->rs_decode_time * 100.0) / total_time;
    double rm_decode_percent = (trace_time->rm_decode_time * 100.0) / total_time;
    double shake256_512_ds_percent = (trace_time->shake256_512_ds_time * 100.0) / total_time;

    // Calculate time in milliseconds (ms)
    double vect_set_random_fixed_weight_ms = trace_time->vect_set_random_fixed_weight_time / static_cast<double>(trace_time->stack) / 1000.0;
    double vect_operation_ms = trace_time->vect_operation_time / static_cast<double>(trace_time->stack) / 1000.0;
    double rs_encode_ms = trace_time->rs_encode_time / static_cast<double>(trace_time->stack) / 1000.0;
    double rm_encode_ms = trace_time->rm_encode_time / static_cast<double>(trace_time->stack) / 1000.0;
    double vect_set_random_ms = trace_time->vect_set_random_time / static_cast<double>(trace_time->stack) / 1000.0;
    double rs_decode_ms = trace_time->rs_decode_time / static_cast<double>(trace_time->stack) / 1000.0;
    double rm_decode_ms = trace_time->rm_decode_time / static_cast<double>(trace_time->stack) / 1000.0;
    double shake256_512_ds_ms = trace_time->shake256_512_ds_time / static_cast<double>(trace_time->stack) / 1000.0;

    // Total time in milliseconds (ms)
    double total_ms = total_time / static_cast<double>(trace_time->stack) / 1000.0;

    // Print percentages and milliseconds in aligned columns
    printf("%-38s %-10s %-10s\n", "Function", "Time (ms)", "Percentage");
    printf("---------------------------------- ---------- ----------\n");
    if (vect_set_random_fixed_weight_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "vect_set_random_fixed_weight:", vect_set_random_fixed_weight_ms, vect_set_random_fixed_weight_percent);
    if (vect_operation_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "vect_operation:", vect_operation_ms, vect_operation_percent);
    if (vect_set_random_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "vect_set_random:", vect_set_random_ms, vect_set_random_percent);
    if (rs_encode_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "[Encode] rs_encode:", rs_encode_ms, rs_encode_percent);
    if (rm_encode_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "[Encode] rm_encode:", rm_encode_ms, rm_encode_percent);
    if (rs_decode_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "[Decode] rs_decode:", rs_decode_ms, rs_decode_percent);
    if (rm_decode_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "[Decode] rm_decode:", rm_decode_ms, rm_decode_percent);
    if (shake256_512_ds_percent > 0)
        printf("%-38s %.6fms (%.2f%%)\n", "shake256_512_ds:", shake256_512_ds_ms, shake256_512_ds_percent);
    
    printf("\nTotal exec time: %.6fms \n", total_ms);
    printf("\n");
}


void rs_decode_detail_analysis(const Trace_time* trace_time) {
    // Calculate total time for RS decoding details
    uint32_t total_time = trace_time->compute_syndromes_time + trace_time->compute_elp_time + trace_time->compute_roots_time +
                          trace_time->compute_z_poly_time + trace_time->compute_error_values_time + trace_time->correct_errors_time;

    // Calculate percentages
    double compute_syndromes_percent = (trace_time->compute_syndromes_time * 100.0) / total_time;
    double compute_elp_percent = (trace_time->compute_elp_time * 100.0) / total_time;
    double compute_roots_percent = (trace_time->compute_roots_time * 100.0) / total_time;
    double compute_z_poly_percent = (trace_time->compute_z_poly_time * 100.0) / total_time;
    double compute_error_values_percent = (trace_time->compute_error_values_time * 100.0) / total_time;
    double correct_errors_percent = (trace_time->correct_errors_time * 100.0) / total_time;

    // Calculate time in milliseconds (ms)
    double compute_syndromes_ms = trace_time->compute_syndromes_time / static_cast<double>(trace_time->stack) / 1000.0;
    double compute_elp_ms = trace_time->compute_elp_time / static_cast<double>(trace_time->stack) / 1000.0;
    double compute_roots_ms = trace_time->compute_roots_time / static_cast<double>(trace_time->stack) / 1000.0;
    double compute_z_poly_ms = trace_time->compute_z_poly_time / static_cast<double>(trace_time->stack) / 1000.0;
    double compute_error_values_ms = trace_time->compute_error_values_time / static_cast<double>(trace_time->stack) / 1000.0;
    double correct_errors_ms = trace_time->correct_errors_time / static_cast<double>(trace_time->stack) / 1000.0;

    // Print details
    printf("\nRS Decode Details Analysis:\n");
    printf("%-38s %-10s %-10s\n", "Function", "Time (ms)", "Percentage");
    printf("---------------------------------- ---------- ----------\n");
    printf("%-38s %.6fms (%.2f%%)\n", "Compute Syndromes:", compute_syndromes_ms, compute_syndromes_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "Compute ELP:", compute_elp_ms, compute_elp_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "Compute Roots:", compute_roots_ms, compute_roots_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "Compute Z Polynomial:", compute_z_poly_ms, compute_z_poly_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "Compute Error Values:", compute_error_values_ms, compute_error_values_percent);
    printf("%-38s %.6fms (%.2f%%)\n", "Correct Errors:", correct_errors_ms, correct_errors_percent);
    printf("\n");
}
