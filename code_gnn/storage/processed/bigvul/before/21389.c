swp_entry_t get_swap_page_of_type(int type)
{
	struct swap_info_struct *si;
	pgoff_t offset;

	spin_lock(&swap_lock);
	si = swap_info[type];
	if (si && (si->flags & SWP_WRITEOK)) {
		nr_swap_pages--;
		 
		offset = scan_swap_map(si, 1);
		if (offset) {
			spin_unlock(&swap_lock);
			return swp_entry(type, offset);
		}
		nr_swap_pages++;
	}
	spin_unlock(&swap_lock);
	return (swp_entry_t) {0};
}
