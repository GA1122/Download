static unsigned char swap_entry_free(struct swap_info_struct *p,
				     swp_entry_t entry, unsigned char usage)
{
	unsigned long offset = swp_offset(entry);
	unsigned char count;
	unsigned char has_cache;

	count = p->swap_map[offset];
	has_cache = count & SWAP_HAS_CACHE;
	count &= ~SWAP_HAS_CACHE;

	if (usage == SWAP_HAS_CACHE) {
		VM_BUG_ON(!has_cache);
		has_cache = 0;
	} else if (count == SWAP_MAP_SHMEM) {
		 
		count = 0;
	} else if ((count & ~COUNT_CONTINUED) <= SWAP_MAP_MAX) {
		if (count == COUNT_CONTINUED) {
			if (swap_count_continued(p, offset, count))
				count = SWAP_MAP_MAX | COUNT_CONTINUED;
			else
				count = SWAP_MAP_MAX;
		} else
			count--;
	}

	if (!count)
		mem_cgroup_uncharge_swap(entry);

	usage = count | has_cache;
	p->swap_map[offset] = usage;

	 
	if (!usage) {
		struct gendisk *disk = p->bdev->bd_disk;
		if (offset < p->lowest_bit)
			p->lowest_bit = offset;
		if (offset > p->highest_bit)
			p->highest_bit = offset;
		if (swap_list.next >= 0 &&
		    p->prio > swap_info[swap_list.next]->prio)
			swap_list.next = p->type;
		nr_swap_pages++;
		p->inuse_pages--;
		if ((p->flags & SWP_BLKDEV) &&
				disk->fops->swap_slot_free_notify)
			disk->fops->swap_slot_free_notify(p->bdev, offset);
	}

	return usage;
}
