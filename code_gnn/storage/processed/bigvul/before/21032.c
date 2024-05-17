static struct page *mc_handle_present_pte(struct vm_area_struct *vma,
						unsigned long addr, pte_t ptent)
{
	struct page *page = vm_normal_page(vma, addr, ptent);

	if (!page || !page_mapped(page))
		return NULL;
	if (PageAnon(page)) {
		 
		if (!move_anon() || page_mapcount(page) > 2)
			return NULL;
	} else if (!move_file())
		 
		return NULL;
	if (!get_page_unless_zero(page))
		return NULL;

	return page;
}
