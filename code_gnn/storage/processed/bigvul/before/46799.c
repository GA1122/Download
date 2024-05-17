static int sha256_sparc64_final(struct shash_desc *desc, u8 *out)
{
	struct sha256_state *sctx = shash_desc_ctx(desc);
	unsigned int i, index, padlen;
	__be32 *dst = (__be32 *)out;
	__be64 bits;
	static const u8 padding[SHA256_BLOCK_SIZE] = { 0x80, };

	bits = cpu_to_be64(sctx->count << 3);

	 
	index = sctx->count % SHA256_BLOCK_SIZE;
	padlen = (index < 56) ? (56 - index) : ((SHA256_BLOCK_SIZE+56) - index);

	 
	if (padlen <= 56) {
		sctx->count += padlen;
		memcpy(sctx->buf + index, padding, padlen);
	} else {
		__sha256_sparc64_update(sctx, padding, padlen, index);
	}
	__sha256_sparc64_update(sctx, (const u8 *)&bits, sizeof(bits), 56);

	 
	for (i = 0; i < 8; i++)
		dst[i] = cpu_to_be32(sctx->state[i]);

	 
	memset(sctx, 0, sizeof(*sctx));

	return 0;
}