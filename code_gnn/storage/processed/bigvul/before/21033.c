static struct page *mc_handle_swap_pte(struct vm_area_struct *vma,
			unsigned long addr, pte_t ptent, swp_entry_t *entry)
{
	int usage_count;
	struct page *page = NULL;
	swp_entry_t ent = pte_to_swp_entry(ptent);

	if (!move_anon() || non_swap_entry(ent))
		return NULL;
	usage_count = mem_cgroup_count_swap_user(ent, &page);
	if (usage_count > 1) {  
		if (page)
			put_page(page);
		return NULL;
	}
	if (do_swap_account)
		entry->val = ent.val;

	return page;
}