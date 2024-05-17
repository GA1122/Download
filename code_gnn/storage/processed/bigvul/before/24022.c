static void emmh32_setseed(emmh32_context *context, u8 *pkey, int keylen,
			   struct crypto_cipher *tfm)
{
   
   
  
	int i,j;
	u32 counter;
	u8 *cipher, plain[16];

	crypto_cipher_setkey(tfm, pkey, 16);
	counter = 0;
	for (i = 0; i < ARRAY_SIZE(context->coeff); ) {
		aes_counter[15] = (u8)(counter >> 0);
		aes_counter[14] = (u8)(counter >> 8);
		aes_counter[13] = (u8)(counter >> 16);
		aes_counter[12] = (u8)(counter >> 24);
		counter++;
		memcpy (plain, aes_counter, 16);
		crypto_cipher_encrypt_one(tfm, plain, plain);
		cipher = plain;
		for (j = 0; (j < 16) && (i < ARRAY_SIZE(context->coeff)); ) {
			context->coeff[i++] = ntohl(*(__be32 *)&cipher[j]);
			j += 4;
		}
	}
}