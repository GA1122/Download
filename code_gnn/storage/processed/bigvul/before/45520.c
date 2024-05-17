static int crypto_authenc_decrypt(struct aead_request *req)
{
	struct crypto_aead *authenc = crypto_aead_reqtfm(req);
	struct crypto_authenc_ctx *ctx = crypto_aead_ctx(authenc);
	struct ablkcipher_request *abreq = aead_request_ctx(req);
	unsigned int cryptlen = req->cryptlen;
	unsigned int authsize = crypto_aead_authsize(authenc);
	u8 *iv = req->iv;
	int err;

	if (cryptlen < authsize)
		return -EINVAL;
	cryptlen -= authsize;

	err = crypto_authenc_iverify(req, iv, cryptlen);
	if (err)
		return err;

	ablkcipher_request_set_tfm(abreq, ctx->enc);
	ablkcipher_request_set_callback(abreq, aead_request_flags(req),
					req->base.complete, req->base.data);
	ablkcipher_request_set_crypt(abreq, req->src, req->dst, cryptlen, iv);

	return crypto_ablkcipher_decrypt(abreq);
}