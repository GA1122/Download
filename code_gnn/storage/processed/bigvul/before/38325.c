static int try_to_unmap_nonlinear(struct page *page,
		struct address_space *mapping, void *arg)
{
	struct vm_area_struct *vma;
	int ret = SWAP_AGAIN;
	unsigned long cursor;
	unsigned long max_nl_cursor = 0;
	unsigned long max_nl_size = 0;
	unsigned int mapcount;

	list_for_each_entry(vma,
		&mapping->i_mmap_nonlinear, shared.nonlinear) {

		cursor = (unsigned long) vma->vm_private_data;
		if (cursor > max_nl_cursor)
			max_nl_cursor = cursor;
		cursor = vma->vm_end - vma->vm_start;
		if (cursor > max_nl_size)
			max_nl_size = cursor;
	}

	if (max_nl_size == 0) {	 
		return SWAP_FAIL;
	}

	 
	mapcount = page_mapcount(page);
	if (!mapcount)
		return ret;

	cond_resched();

	max_nl_size = (max_nl_size + CLUSTER_SIZE - 1) & CLUSTER_MASK;
	if (max_nl_cursor == 0)
		max_nl_cursor = CLUSTER_SIZE;

	do {
		list_for_each_entry(vma,
			&mapping->i_mmap_nonlinear, shared.nonlinear) {

			cursor = (unsigned long) vma->vm_private_data;
			while (cursor < max_nl_cursor &&
				cursor < vma->vm_end - vma->vm_start) {
				if (try_to_unmap_cluster(cursor, &mapcount,
						vma, page) == SWAP_MLOCK)
					ret = SWAP_MLOCK;
				cursor += CLUSTER_SIZE;
				vma->vm_private_data = (void *) cursor;
				if ((int)mapcount <= 0)
					return ret;
			}
			vma->vm_private_data = (void *) max_nl_cursor;
		}
		cond_resched();
		max_nl_cursor += CLUSTER_SIZE;
	} while (max_nl_cursor <= max_nl_size);

	 
	list_for_each_entry(vma, &mapping->i_mmap_nonlinear, shared.nonlinear)
		vma->vm_private_data = NULL;

	return ret;
}