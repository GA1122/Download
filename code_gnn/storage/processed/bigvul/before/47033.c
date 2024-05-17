static int sha1_ssse3_final(struct shash_desc *desc, u8 *out)
{
	struct sha1_state *sctx = shash_desc_ctx(desc);
	unsigned int i, index, padlen;
	__be32 *dst = (__be32 *)out;
	__be64 bits;
	static const u8 padding[SHA1_BLOCK_SIZE] = { 0x80, };

	bits = cpu_to_be64(sctx->count << 3);

	 
	index = sctx->count % SHA1_BLOCK_SIZE;
	padlen = (index < 56) ? (56 - index) : ((SHA1_BLOCK_SIZE+56) - index);
	if (!irq_fpu_usable()) {
		crypto_sha1_update(desc, padding, padlen);
		crypto_sha1_update(desc, (const u8 *)&bits, sizeof(bits));
	} else {
		kernel_fpu_begin();
		 
		if (padlen <= 56) {
			sctx->count += padlen;
			memcpy(sctx->buffer + index, padding, padlen);
		} else {
			__sha1_ssse3_update(desc, padding, padlen, index);
		}
		__sha1_ssse3_update(desc, (const u8 *)&bits, sizeof(bits), 56);
		kernel_fpu_end();
	}

	 
	for (i = 0; i < 5; i++)
		dst[i] = cpu_to_be32(sctx->state[i]);

	 
	memset(sctx, 0, sizeof(*sctx));

	return 0;
}