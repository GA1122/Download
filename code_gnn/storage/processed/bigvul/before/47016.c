static void decrypt_callback(void *priv, u8 *srcdst, unsigned int nbytes)
{
	const unsigned int bsize = SERPENT_BLOCK_SIZE;
	struct crypt_priv *ctx = priv;
	int i;

	ctx->fpu_enabled = serpent_fpu_begin(ctx->fpu_enabled, nbytes);

	if (nbytes == bsize * SERPENT_PARALLEL_BLOCKS) {
		serpent_dec_blk_xway(ctx->ctx, srcdst, srcdst);
		return;
	}

	for (i = 0; i < nbytes / bsize; i++, srcdst += bsize)
		__serpent_decrypt(ctx->ctx, srcdst, srcdst);
}