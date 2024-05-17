static int lz4hc_compress_crypto(struct crypto_tfm *tfm, const u8 *src,
			    unsigned int slen, u8 *dst, unsigned int *dlen)
{
	struct lz4hc_ctx *ctx = crypto_tfm_ctx(tfm);
	size_t tmp_len = *dlen;
	int err;

	err = lz4hc_compress(src, slen, dst, &tmp_len, ctx->lz4hc_comp_mem);

	if (err < 0)
		return -EINVAL;

	*dlen = tmp_len;
	return 0;
}