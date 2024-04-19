#include <stdio.h>
#include <time.h>
#include "api.h"
#include "parameters.h"

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
	int keygen_t = 0; 
	int encrypt_t = 0;
	int decrypt_t = 0;

	clock_t start, end;
	int cpu_time_used;

	for (int i = 0; i < iter; i++) {

	start = clock();
	crypto_kem_keypair(pk, sk);
	end = clock();
	cpu_time_used = ((int) (end - start));
	keygen_t += cpu_time_used;

	start = clock();
	crypto_kem_enc(ct, key1, pk);
	end = clock();
	cpu_time_used = ((int) (end - start));
	encrypt_t += cpu_time_used;

	start = clock();
	crypto_kem_dec(key2, ct, sk);
	end = clock();
	cpu_time_used = ((int) (end - start));
	decrypt_t += cpu_time_used;
	}

	printf("\n keygen time: %f msec", ((double)keygen_t) / (double)iter / CLOCKS_PER_SEC * 1000);
	printf("\n encrypt time: %f msec", ((double)encrypt_t) / (double)iter / CLOCKS_PER_SEC * 1000);
	printf("\n decrypt time: %f msec", ((double)decrypt_t) / (double)iter / CLOCKS_PER_SEC * 1000);


	printf("\n\nsecret1: ");
	for(int i = 0 ; i < SHARED_SECRET_BYTES ; ++i) printf("%x", key1[i]);

	printf("\nsecret2: ");
	for(int i = 0 ; i < SHARED_SECRET_BYTES ; ++i) printf("%x", key2[i]);
	printf("\n\n");

	return 0;
}