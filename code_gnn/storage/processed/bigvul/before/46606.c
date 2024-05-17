int sha1_update_arm(struct shash_desc *desc, const u8 *data,
		    unsigned int len)
{
	struct sha1_state *sctx = shash_desc_ctx(desc);
	unsigned int partial = sctx->count % SHA1_BLOCK_SIZE;
	int res;

	 
	if (partial + len < SHA1_BLOCK_SIZE) {
		sctx->count += len;
		memcpy(sctx->buffer + partial, data, len);
		return 0;
	}
	res = __sha1_update(sctx, data, len, partial);
	return res;
}