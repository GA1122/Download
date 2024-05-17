ulong post_word_load(void)
{
	void* addr = (void *) (gd->ram_size - BOOTCOUNT_ADDR + POST_WORD_OFF);
	return in_le32(addr);

}
