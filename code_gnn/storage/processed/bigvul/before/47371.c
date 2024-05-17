int crypto_sha512_update(struct shash_desc *desc, const u8 *data,
			unsigned int len)
{
	struct sha512_state *sctx = shash_desc_ctx(desc);

	unsigned int i, index, part_len;

	 
	index = sctx->count[0] & 0x7f;

	 
	if ((sctx->count[0] += len) < len)
		sctx->count[1]++;

        part_len = 128 - index;

	 
	if (len >= part_len) {
		memcpy(&sctx->buf[index], data, part_len);
		sha512_transform(sctx->state, sctx->buf);

		for (i = part_len; i + 127 < len; i+=128)
			sha512_transform(sctx->state, &data[i]);

		index = 0;
	} else {
		i = 0;
	}

	 
	memcpy(&sctx->buf[index], &data[i], len - i);

	return 0;
}
