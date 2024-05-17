static const EVP_CIPHER *get_cipher_for_algo(int alg_id)
{
	switch (alg_id) {
		case 0x0: return EVP_des_ede3_ecb();
		case 0x1: return EVP_des_ede3_ecb();  
		case 0x3: return EVP_des_ede3_ecb();
		case 0x8: return EVP_aes_128_ecb();
		case 0xA: return EVP_aes_192_ecb();
		case 0xC: return EVP_aes_256_ecb();
		default: return NULL;
	}
}