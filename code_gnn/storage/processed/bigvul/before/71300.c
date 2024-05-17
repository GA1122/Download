 static int mcryptd_hash_init_tfm(struct crypto_tfm *tfm)
{
	struct crypto_instance *inst = crypto_tfm_alg_instance(tfm);
	struct hashd_instance_ctx *ictx = crypto_instance_ctx(inst);
	struct crypto_ahash_spawn *spawn = &ictx->spawn;
	struct mcryptd_hash_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_ahash *hash;

	hash = crypto_spawn_ahash(spawn);
	if (IS_ERR(hash))
		return PTR_ERR(hash);

	ctx->child = hash;
	crypto_ahash_set_reqsize(__crypto_ahash_cast(tfm),
				 sizeof(struct mcryptd_hash_request_ctx) +
				 crypto_ahash_reqsize(hash));
	return 0;
}