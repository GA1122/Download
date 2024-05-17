static unsigned long get_unmapped_area_zero(struct file *file,
				unsigned long addr, unsigned long len,
				unsigned long pgoff, unsigned long flags)
{
#ifdef CONFIG_MMU
	if (flags & MAP_SHARED) {
		 
		return shmem_get_unmapped_area(NULL, addr, len, pgoff, flags);
	}

	 
	return current->mm->get_unmapped_area(file, addr, len, pgoff, flags);
#else
	return -ENOSYS;
#endif
}
