static struct page *mc_handle_file_pte(struct vm_area_struct *vma,
			unsigned long addr, pte_t ptent, swp_entry_t *entry)
{
	struct page *page = NULL;
	struct inode *inode;
	struct address_space *mapping;
	pgoff_t pgoff;

	if (!vma->vm_file)  
		return NULL;
	if (!move_file())
		return NULL;

	inode = vma->vm_file->f_path.dentry->d_inode;
	mapping = vma->vm_file->f_mapping;
	if (pte_none(ptent))
		pgoff = linear_page_index(vma, addr);
	else  
		pgoff = pte_to_pgoff(ptent);

	 
	page = find_get_page(mapping, pgoff);

#ifdef CONFIG_SWAP
	 
	if (radix_tree_exceptional_entry(page)) {
		swp_entry_t swap = radix_to_swp_entry(page);
		if (do_swap_account)
			*entry = swap;
		page = find_get_page(&swapper_space, swap.val);
	}
#endif
	return page;
}
