static int ahash_update(struct ahash_request *req)
{
	int ret = 0;
	struct hash_req_ctx *req_ctx = ahash_request_ctx(req);

	if (hash_mode != HASH_MODE_DMA || !req_ctx->dma_mode)
		ret = hash_hw_update(req);
	 

	if (ret) {
		pr_err("%s: hash_hw_update() failed!\n", __func__);
	}

	return ret;
}
