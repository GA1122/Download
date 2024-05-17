ulong board_init_f_alloc_reserve(ulong top)
{
	 
#if CONFIG_VAL(SYS_MALLOC_F_LEN)
	top -= CONFIG_VAL(SYS_MALLOC_F_LEN);
#endif
	 
	top = rounddown(top-sizeof(struct global_data), 16);

	return top;
}
