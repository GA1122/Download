static void security_salted_hash(const BYTE* salt, const BYTE* input, int length,
		const BYTE* salt1, const BYTE* salt2, BYTE* output)
{
	CryptoMd5 md5;
	CryptoSha1 sha1;
	BYTE sha1_digest[CRYPTO_SHA1_DIGEST_LENGTH];

	 

	 
	sha1 = crypto_sha1_init();
	crypto_sha1_update(sha1, input, length);  
	crypto_sha1_update(sha1, salt, 48);  
	crypto_sha1_update(sha1, salt1, 32);  
	crypto_sha1_update(sha1, salt2, 32);  
	crypto_sha1_final(sha1, sha1_digest);

	 
	md5 = crypto_md5_init();
	crypto_md5_update(md5, salt, 48);  
	crypto_md5_update(md5, sha1_digest, sizeof(sha1_digest));  
	crypto_md5_final(md5, output);
}
