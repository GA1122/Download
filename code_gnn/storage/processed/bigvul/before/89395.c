		func_ptr_to_num(void *func_ptr)
{
	uintptr_t offset = (uintptr_t)func_ptr;

#ifdef CONFIG_SANDBOX
	offset -= (uintptr_t)&_init;
#else
	if (gd->flags & GD_FLG_RELOC)
		offset -= gd->relocaddr;
	else
		offset -= CONFIG_SYS_TEXT_BASE;
#endif
	return offset / FUNC_SITE_SIZE;
}
