void security_mac_signature(rdpRdp *rdp, const BYTE* data, UINT32 length, BYTE* output)
{
	CryptoMd5 md5;
	CryptoSha1 sha1;
	BYTE length_le[4];
	BYTE md5_digest[CRYPTO_MD5_DIGEST_LENGTH];
	BYTE sha1_digest[CRYPTO_SHA1_DIGEST_LENGTH];

	security_UINT32_le(length_le, length);  

	 
	sha1 = crypto_sha1_init();
	crypto_sha1_update(sha1, rdp->sign_key, rdp->rc4_key_len);  
	crypto_sha1_update(sha1, pad1, sizeof(pad1));  
	crypto_sha1_update(sha1, length_le, sizeof(length_le));  
	crypto_sha1_update(sha1, data, length);  
	crypto_sha1_final(sha1, sha1_digest);

	 
	md5 = crypto_md5_init();
	crypto_md5_update(md5, rdp->sign_key, rdp->rc4_key_len);  
	crypto_md5_update(md5, pad2, sizeof(pad2));  
	crypto_md5_update(md5, sha1_digest, sizeof(sha1_digest));  
	crypto_md5_final(md5, md5_digest);

	memcpy(output, md5_digest, 8);
}
