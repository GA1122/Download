static int sha224_ssse3_final(struct shash_desc *desc, u8 *hash)
{
	u8 D[SHA256_DIGEST_SIZE];

	sha256_ssse3_final(desc, D);

	memcpy(hash, D, SHA224_DIGEST_SIZE);
	memset(D, 0, SHA256_DIGEST_SIZE);

	return 0;
}