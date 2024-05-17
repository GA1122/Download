static void xfer_secondary_pool(struct entropy_store *r, size_t nbytes)
{
	__u32 tmp[OUTPUT_POOL_WORDS];

	if (r->pull && r->entropy_count < nbytes * 8 &&
	    r->entropy_count < r->poolinfo->POOLBITS) {
		 
		int rsvd = r->limit ? 0 : random_read_wakeup_thresh/4;
		int bytes = nbytes;

		 
		bytes = max_t(int, bytes, random_read_wakeup_thresh / 8);
		 
		bytes = min_t(int, bytes, sizeof(tmp));

		DEBUG_ENT("going to reseed %s with %d bits "
			  "(%d of %d requested)\n",
			  r->name, bytes * 8, nbytes * 8, r->entropy_count);

		bytes = extract_entropy(r->pull, tmp, bytes,
					random_read_wakeup_thresh / 8, rsvd);
		mix_pool_bytes(r, tmp, bytes);
		credit_entropy_bits(r, bytes*8);
	}
}
