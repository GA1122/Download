SYSCALL_DEFINE3(mincore, unsigned long, start, size_t, len,
		unsigned char __user *, vec)
{
	long retval;
	unsigned long pages;
	unsigned char *tmp;

	 
 	if (start & ~PAGE_CACHE_MASK)
		return -EINVAL;

	 
	if (!access_ok(VERIFY_READ, (void __user *) start, len))
		return -ENOMEM;

	 
	pages = len >> PAGE_SHIFT;
	pages += (len & ~PAGE_MASK) != 0;

	if (!access_ok(VERIFY_WRITE, vec, pages))
		return -EFAULT;

	tmp = (void *) __get_free_page(GFP_USER);
	if (!tmp)
		return -EAGAIN;

	retval = 0;
	while (pages) {
		 
		down_read(&current->mm->mmap_sem);
		retval = do_mincore(start, min(pages, PAGE_SIZE), tmp);
		up_read(&current->mm->mmap_sem);

		if (retval <= 0)
			break;
		if (copy_to_user(vec, tmp, retval)) {
			retval = -EFAULT;
			break;
		}
		pages -= retval;
		vec += retval;
		start += retval << PAGE_SHIFT;
		retval = 0;
	}
	free_page((unsigned long) tmp);
	return retval;
}