int get_user_pages_fast(unsigned long start, int nr_pages, int write,
			struct page **pages)
{
	int nr, ret;

	start &= PAGE_MASK;
	nr = __get_user_pages_fast(start, nr_pages, write, pages);
	ret = nr;

	if (nr < nr_pages) {
		 
		start += nr << PAGE_SHIFT;
		pages += nr;

		ret = get_user_pages_unlocked(start, nr_pages - nr, write, 0, pages);

		 
		if (nr > 0) {
			if (ret < 0)
				ret = nr;
			else
				ret += nr;
		}
	}

	return ret;
}