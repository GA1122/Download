int __get_user_pages_fast(unsigned long start, int nr_pages, int write,
			  struct page **pages)
{
	unsigned long len, end;
	unsigned long flags;
	int nr = 0;

	start &= PAGE_MASK;
	len = (unsigned long) nr_pages << PAGE_SHIFT;
	end = start + len;

	if (unlikely(!access_ok((void __user *)start, len)))
		return 0;

	 

	if (gup_fast_permitted(start, nr_pages)) {
		local_irq_save(flags);
		gup_pgd_range(start, end, write, pages, &nr);
		local_irq_restore(flags);
	}

	return nr;
}
