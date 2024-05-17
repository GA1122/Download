static void aes_encrypt(struct crypto_tfm *tfm, u8 *out, const u8 *in)
{
	struct aes_ctx *ctx = aes_ctx(tfm);
	int ts_state;

	padlock_reset_key(&ctx->cword.encrypt);
	ts_state = irq_ts_save();
	ecb_crypt(in, out, ctx->E, &ctx->cword.encrypt, 1);
	irq_ts_restore(ts_state);
	padlock_store_cword(&ctx->cword.encrypt);
}