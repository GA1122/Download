static int mcryptd_hash_export(struct ahash_request *req, void *out)
{
	struct mcryptd_hash_request_ctx *rctx = ahash_request_ctx(req);

	return crypto_ahash_export(&rctx->areq, out);
}