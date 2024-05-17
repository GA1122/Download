int xts_camellia_setkey(struct crypto_tfm *tfm, const u8 *key,
			unsigned int keylen)
{
	struct camellia_xts_ctx *ctx = crypto_tfm_ctx(tfm);
	u32 *flags = &tfm->crt_flags;
	int err;

	 
	if (keylen % 2) {
		*flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}

	 
	err = __camellia_setkey(&ctx->crypt_ctx, key, keylen / 2, flags);
	if (err)
		return err;

	 
	return __camellia_setkey(&ctx->tweak_ctx, key + keylen / 2, keylen / 2,
				flags);
}