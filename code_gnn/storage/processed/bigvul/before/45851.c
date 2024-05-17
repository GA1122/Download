int shash_ahash_mcryptd_update(struct ahash_request *req,
			       struct shash_desc *desc)
{
	struct crypto_shash *tfm = desc->tfm;
	struct shash_alg *shash = crypto_shash_alg(tfm);

	 

	return shash->update(desc, NULL, 0);
}
