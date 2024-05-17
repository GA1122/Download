void swapcache_free(swp_entry_t entry, struct page *page)
{
	struct swap_info_struct *p;
	unsigned char count;

	p = swap_info_get(entry);
	if (p) {
		count = swap_entry_free(p, entry, SWAP_HAS_CACHE);
		if (page)
			mem_cgroup_uncharge_swapcache(page, entry, count != 0);
		spin_unlock(&swap_lock);
	}
}
