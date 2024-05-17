int get_user_pages_fast(unsigned long start, int nr_pages, int write,
			struct page **pages)
{
	unsigned long addr, len, end;
	int nr = 0, ret = 0;

	start &= PAGE_MASK;
	addr = start;
	len = (unsigned long) nr_pages << PAGE_SHIFT;
	end = start + len;

	if (nr_pages <= 0)
		return 0;

	if (unlikely(!access_ok((void __user *)start, len)))
		return -EFAULT;

	if (gup_fast_permitted(start, nr_pages, write)) {
		local_irq_disable();
		gup_pgd_range(addr, end, write, pages, &nr);
		local_irq_enable();
		ret = nr;
	}

	if (nr < nr_pages) {
		 
		start += nr << PAGE_SHIFT;
		pages += nr;

		ret = get_user_pages_unlocked(start, nr_pages - nr, pages,
				write ? FOLL_WRITE : 0);

		 
		if (nr > 0) {
			if (ret < 0)
				ret = nr;
			else
				ret += nr;
		}
	}

	return ret;
}
