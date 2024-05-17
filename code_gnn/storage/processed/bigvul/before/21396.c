static unsigned long read_swap_header(struct swap_info_struct *p,
					union swap_header *swap_header,
					struct inode *inode)
{
	int i;
	unsigned long maxpages;
	unsigned long swapfilepages;

	if (memcmp("SWAPSPACE2", swap_header->magic.magic, 10)) {
		printk(KERN_ERR "Unable to find swap-space signature\n");
		return 0;
	}

	 
	if (swab32(swap_header->info.version) == 1) {
		swab32s(&swap_header->info.version);
		swab32s(&swap_header->info.last_page);
		swab32s(&swap_header->info.nr_badpages);
		for (i = 0; i < swap_header->info.nr_badpages; i++)
			swab32s(&swap_header->info.badpages[i]);
	}
	 
	if (swap_header->info.version != 1) {
		printk(KERN_WARNING
		       "Unable to handle swap header version %d\n",
		       swap_header->info.version);
		return 0;
	}

	p->lowest_bit  = 1;
	p->cluster_next = 1;
	p->cluster_nr = 0;

	 
	maxpages = swp_offset(pte_to_swp_entry(
			swp_entry_to_pte(swp_entry(0, ~0UL))));
	maxpages = swp_offset(radix_to_swp_entry(
			swp_to_radix_entry(swp_entry(0, maxpages)))) + 1;

	if (maxpages > swap_header->info.last_page) {
		maxpages = swap_header->info.last_page + 1;
		 
		if ((unsigned int)maxpages == 0)
			maxpages = UINT_MAX;
	}
	p->highest_bit = maxpages - 1;

	if (!maxpages)
		return 0;
	swapfilepages = i_size_read(inode) >> PAGE_SHIFT;
	if (swapfilepages && maxpages > swapfilepages) {
		printk(KERN_WARNING
		       "Swap area shorter than signature indicates\n");
		return 0;
	}
	if (swap_header->info.nr_badpages && S_ISREG(inode->i_mode))
		return 0;
	if (swap_header->info.nr_badpages > MAX_SWAP_BADPAGES)
		return 0;

	return maxpages;
}
