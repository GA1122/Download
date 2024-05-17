static inline int rcu_use_vmalloc(int size)
{
	 
	if (HDRLEN_KMALLOC + size > PAGE_SIZE)
		return 1;
	return 0;
}
