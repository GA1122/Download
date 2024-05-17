int ahash_mcryptd_final(struct ahash_request *desc)
{
	 

	return crypto_ahash_final(desc);
}
