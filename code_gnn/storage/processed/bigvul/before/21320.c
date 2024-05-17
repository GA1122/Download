static int migrate_to_node(struct mm_struct *mm, int source, int dest,
			   int flags)
{
	nodemask_t nmask;
	LIST_HEAD(pagelist);
	int err = 0;
	struct vm_area_struct *vma;

	nodes_clear(nmask);
	node_set(source, nmask);

	vma = check_range(mm, mm->mmap->vm_start, mm->task_size, &nmask,
			flags | MPOL_MF_DISCONTIG_OK, &pagelist);
	if (IS_ERR(vma))
		return PTR_ERR(vma);

	if (!list_empty(&pagelist)) {
		err = migrate_pages(&pagelist, new_node_page, dest,
							false, MIGRATE_SYNC);
		if (err)
			putback_lru_pages(&pagelist);
	}

	return err;
}
