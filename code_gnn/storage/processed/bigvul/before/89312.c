void post_word_store(ulong value)
{
	void* addr = (void *) (gd->ram_size - BOOTCOUNT_ADDR + POST_WORD_OFF);
	out_le32(addr, value);
}
