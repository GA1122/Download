static ssize_t do_write_kmem(unsigned long p, const char __user *buf,
				size_t count, loff_t *ppos)
{
	ssize_t written, sz;
	unsigned long copied;

	written = 0;
#ifdef __ARCH_HAS_NO_PAGE_ZERO_MAPPED
	 
	if (p < PAGE_SIZE) {
		sz = size_inside_page(p, count);
		 
		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
	}
#endif

	while (count > 0) {
		void *ptr;

		sz = size_inside_page(p, count);

		 
		ptr = xlate_dev_kmem_ptr((void *)p);
		if (!virt_addr_valid(ptr))
			return -ENXIO;

		copied = copy_from_user(ptr, buf, sz);
		if (copied) {
			written += sz - copied;
			if (written)
				break;
			return -EFAULT;
		}
		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
	}

	*ppos += written;
	return written;
}