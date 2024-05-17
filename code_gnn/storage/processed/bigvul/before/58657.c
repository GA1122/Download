BOOL security_establish_keys(const BYTE* client_random, rdpRdp* rdp)
{
	BYTE pre_master_secret[48];
	BYTE master_secret[48];
	BYTE session_key_blob[48];
	BYTE* server_random;
	BYTE salt40[] = { 0xD1, 0x26, 0x9E };
	rdpSettings* settings;

	settings = rdp->settings;
	server_random = settings->ServerRandom;

	if (settings->EncryptionMethods == ENCRYPTION_METHOD_FIPS)
	{
		CryptoSha1 sha1;
		BYTE client_encrypt_key_t[CRYPTO_SHA1_DIGEST_LENGTH + 1];
		BYTE client_decrypt_key_t[CRYPTO_SHA1_DIGEST_LENGTH + 1];

		printf("FIPS Compliant encryption level.\n");

		 
		rdp->settings->FastPathInput = FALSE;

		sha1 = crypto_sha1_init();
		crypto_sha1_update(sha1, client_random + 16, 16);
		crypto_sha1_update(sha1, server_random + 16, 16);
		crypto_sha1_final(sha1, client_encrypt_key_t);

		client_encrypt_key_t[20] = client_encrypt_key_t[0];
		fips_expand_key_bits(client_encrypt_key_t, rdp->fips_encrypt_key);

		sha1 = crypto_sha1_init();
		crypto_sha1_update(sha1, client_random, 16);
		crypto_sha1_update(sha1, server_random, 16);
		crypto_sha1_final(sha1, client_decrypt_key_t);

		client_decrypt_key_t[20] = client_decrypt_key_t[0];
		fips_expand_key_bits(client_decrypt_key_t, rdp->fips_decrypt_key);

		sha1 = crypto_sha1_init();
		crypto_sha1_update(sha1, client_decrypt_key_t, 20);
		crypto_sha1_update(sha1, client_encrypt_key_t, 20);
		crypto_sha1_final(sha1, rdp->fips_sign_key);
	}

	memcpy(pre_master_secret, client_random, 24);
	memcpy(pre_master_secret + 24, server_random, 24);

	security_A(pre_master_secret, client_random, server_random, master_secret);
	security_X(master_secret, client_random, server_random, session_key_blob);

	memcpy(rdp->sign_key, session_key_blob, 16);

	if (rdp->settings->ServerMode)
	{
		security_md5_16_32_32(&session_key_blob[16], client_random,
		    server_random, rdp->encrypt_key);
		security_md5_16_32_32(&session_key_blob[32], client_random,
		    server_random, rdp->decrypt_key);
	}
	else
	{
		security_md5_16_32_32(&session_key_blob[16], client_random,
		    server_random, rdp->decrypt_key);
		security_md5_16_32_32(&session_key_blob[32], client_random,
		    server_random, rdp->encrypt_key);
	}

	if (settings->EncryptionMethods == 1)  
	{
		memcpy(rdp->sign_key, salt40, 3);  
		memcpy(rdp->decrypt_key, salt40, 3);  
		memcpy(rdp->encrypt_key, salt40, 3);  
		rdp->rc4_key_len = 8;
	}
	else if (settings->EncryptionMethods == 2)  
	{
		rdp->rc4_key_len = 16;
	}

	memcpy(rdp->decrypt_update_key, rdp->decrypt_key, 16);
	memcpy(rdp->encrypt_update_key, rdp->encrypt_key, 16);
	rdp->decrypt_use_count = 0;
	rdp->decrypt_checksum_use_count = 0;
	rdp->encrypt_use_count =0;
	rdp->encrypt_checksum_use_count =0;

	return TRUE;
}