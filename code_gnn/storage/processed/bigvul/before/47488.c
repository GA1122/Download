static int cryp_blk_encrypt(struct ablkcipher_request *areq)
{
	struct crypto_ablkcipher *cipher = crypto_ablkcipher_reqtfm(areq);
	struct cryp_ctx *ctx = crypto_ablkcipher_ctx(cipher);

	pr_debug(DEV_DBG_NAME " [%s]", __func__);

	ctx->config.algodir = CRYP_ALGORITHM_ENCRYPT;

	 
	if (cryp_mode == CRYP_MODE_DMA && mode_is_aes(ctx->config.algomode))
		return ablk_dma_crypt(areq);

	 
	return ablk_crypt(areq);
}
