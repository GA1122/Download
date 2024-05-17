static int crypto_ccm_auth(struct aead_request *req, struct scatterlist *plain,
			   unsigned int cryptlen)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_ccm_ctx *ctx = crypto_aead_ctx(aead);
	struct crypto_ccm_req_priv_ctx *pctx = crypto_ccm_reqctx(req);
	struct crypto_cipher *cipher = ctx->cipher;
	unsigned int assoclen = req->assoclen;
	u8 *odata = pctx->odata;
	u8 *idata = pctx->idata;
	int err;

	 
	err = format_input(odata, req, cryptlen);
	if (err)
		goto out;

	 
	crypto_cipher_encrypt_one(cipher, odata, odata);

	 
	if (assoclen) {
		pctx->ilen = format_adata(idata, assoclen);
		get_data_to_compute(cipher, pctx, req->assoc, req->assoclen);
	} else {
		pctx->ilen = 0;
	}

	 
	if (cryptlen)
		get_data_to_compute(cipher, pctx, plain, cryptlen);

out:
	return err;
}
