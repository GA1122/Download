static int sha256_final(struct shash_desc *desc, u8 *out)
{
	struct sha256_state *sctx = shash_desc_ctx(desc);
	__be32 *dst = (__be32 *)out;
	__be64 bits;
	unsigned int index, pad_len;
	int i;
	static const u8 padding[64] = { 0x80, };

	 
	bits = cpu_to_be64(sctx->count << 3);

	 
	index = sctx->count & 0x3f;
	pad_len = (index < 56) ? (56 - index) : ((64+56) - index);
	crypto_sha256_update(desc, padding, pad_len);

	 
	crypto_sha256_update(desc, (const u8 *)&bits, sizeof(bits));

	 
	for (i = 0; i < 8; i++)
		dst[i] = cpu_to_be32(sctx->state[i]);

	 
	memset(sctx, 0, sizeof(*sctx));

	return 0;
}