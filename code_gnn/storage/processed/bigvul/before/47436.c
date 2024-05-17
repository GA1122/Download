static inline void ecb_crypt(const u8 *in, u8 *out, u32 *key,
			     struct cword *cword, int count)
{
	 
	if (unlikely(((unsigned long)in & ~PAGE_MASK) + ecb_fetch_bytes > PAGE_SIZE)) {
		ecb_crypt_copy(in, out, key, cword, count);
		return;
	}

	rep_xcrypt_ecb(in, out, key, cword, count);
}
