static void ecb_crypt_copy(const u8 *in, u8 *out, u32 *key,
			   struct cword *cword, int count)
{
	 
	u8 buf[AES_BLOCK_SIZE * (MAX_ECB_FETCH_BLOCKS - 1) + PADLOCK_ALIGNMENT - 1];
	u8 *tmp = PTR_ALIGN(&buf[0], PADLOCK_ALIGNMENT);

	memcpy(tmp, in, count * AES_BLOCK_SIZE);
	rep_xcrypt_ecb(tmp, out, key, cword, count);
}