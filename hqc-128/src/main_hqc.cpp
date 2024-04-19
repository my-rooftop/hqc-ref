#include <stdio.h>
#include <time.h>
#include "api.h"
#include "parameters.h"
#include "profiling.h"

int main() {

	printf("\n");
	printf("*********************\n");
	printf("**** HQC-%d-%d ****\n", PARAM_SECURITY, PARAM_DFR_EXP);
	printf("*********************\n");

	printf("\n");
	printf("N: %d   ", PARAM_N);
	printf("N1: %d   ", PARAM_N1);
	printf("N2: %d   ", PARAM_N2);
	printf("OMEGA: %d   ", PARAM_OMEGA);
	printf("OMEGA_R: %d   ", PARAM_OMEGA_R);
	printf("Failure rate: 2^-%d   ", PARAM_DFR_EXP);
	printf("Sec: %d bits", PARAM_SECURITY);
	printf("\n");

	unsigned char pk[PUBLIC_KEY_BYTES];
	unsigned char sk[SECRET_KEY_BYTES];
	unsigned char ct[CIPHERTEXT_BYTES];
	unsigned char key1[SHARED_SECRET_BYTES];
	unsigned char key2[SHARED_SECRET_BYTES];
	
	int iter = 20000; 

	Trace_time keygen_time;
	Trace_time encap_time;
	Trace_time decap_time;

	for (int i = 0; i < iter; i++) {

		crypto_kem_keypair(pk, sk, &keygen_time);

		crypto_kem_enc(ct, key1, pk, &encap_time);

		crypto_kem_dec(key2, ct, sk, &decap_time);
	}

	printf("\nkeygen\n");
	time_analysis(&keygen_time);
	printf("encap \n");
	time_analysis(&encap_time);
	printf("decap \n");
	time_analysis(&decap_time);
	printf("rs-decode details \n");
	rs_decode_detail_analysis(&decap_time);


	printf("\n\nsecret1: ");
	for(int i = 0 ; i < SHARED_SECRET_BYTES ; ++i) printf("%x", key1[i]);

	printf("\nsecret2: ");
	for(int i = 0 ; i < SHARED_SECRET_BYTES ; ++i) printf("%x", key2[i]);
	printf("\n\n");

	return 0;
}
