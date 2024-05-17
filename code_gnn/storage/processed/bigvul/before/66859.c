void __init free_initrd_mem(unsigned long start, unsigned long end)
{
	 
	free_init_pages("initrd", start, PAGE_ALIGN(end));
}
