int mem_cgroup_count_swap_user(swp_entry_t ent, struct page **pagep)
{
	struct page *page;
	struct swap_info_struct *p;
	int count = 0;

	page = find_get_page(&swapper_space, ent.val);
	if (page)
		count += page_mapcount(page);
	p = swap_info_get(ent);
	if (p) {
		count += swap_count(p->swap_map[swp_offset(ent)]);
		spin_unlock(&swap_lock);
	}

	*pagep = page;
	return count;
}
