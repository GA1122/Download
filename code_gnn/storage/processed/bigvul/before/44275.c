static unsigned long mmap_rnd(void)
{
	unsigned long rnd = 0;

	 
	if (current->flags & PF_RANDOMIZE) {
		if (mmap_is_ia32())
			rnd = get_random_int() % (1<<8);
		else
			rnd = get_random_int() % (1<<28);
	}
	return rnd << PAGE_SHIFT;
}
