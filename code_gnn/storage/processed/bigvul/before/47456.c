static int padlock_sha256_final_nano(struct shash_desc *desc, u8 *out)
{
	struct sha256_state *state =
		(struct sha256_state *)shash_desc_ctx(desc);
	unsigned int partial, padlen;
	__be64 bits;
	static const u8 padding[64] = { 0x80, };

	bits = cpu_to_be64(state->count << 3);

	 
	partial = state->count & 0x3f;
	padlen = (partial < 56) ? (56 - partial) : ((64+56) - partial);
	padlock_sha256_update_nano(desc, padding, padlen);

	 
	padlock_sha256_update_nano(desc, (const u8 *)&bits, sizeof(bits));

	 
	padlock_output_block((uint32_t *)(state->state), (uint32_t *)out, 8);

	return 0;
}