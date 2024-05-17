static int __init floppy_init(void)
{
#ifdef MODULE
	return do_floppy_init();
#else
	 
	async_schedule(floppy_async_init, NULL);
	return 0;
#endif
}
