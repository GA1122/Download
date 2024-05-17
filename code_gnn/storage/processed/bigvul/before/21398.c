static unsigned long scan_swap_map(struct swap_info_struct *si,
				   unsigned char usage)
{
	unsigned long offset;
	unsigned long scan_base;
	unsigned long last_in_cluster = 0;
	int latency_ration = LATENCY_LIMIT;
	int found_free_cluster = 0;

	 

	si->flags += SWP_SCANNING;
	scan_base = offset = si->cluster_next;

	if (unlikely(!si->cluster_nr--)) {
		if (si->pages - si->inuse_pages < SWAPFILE_CLUSTER) {
			si->cluster_nr = SWAPFILE_CLUSTER - 1;
			goto checks;
		}
		if (si->flags & SWP_DISCARDABLE) {
			 
			if (si->lowest_alloc)
				goto checks;
			si->lowest_alloc = si->max;
			si->highest_alloc = 0;
		}
		spin_unlock(&swap_lock);

		 
		if (!(si->flags & SWP_SOLIDSTATE))
			scan_base = offset = si->lowest_bit;
		last_in_cluster = offset + SWAPFILE_CLUSTER - 1;

		 
		for (; last_in_cluster <= si->highest_bit; offset++) {
			if (si->swap_map[offset])
				last_in_cluster = offset + SWAPFILE_CLUSTER;
			else if (offset == last_in_cluster) {
				spin_lock(&swap_lock);
				offset -= SWAPFILE_CLUSTER - 1;
				si->cluster_next = offset;
				si->cluster_nr = SWAPFILE_CLUSTER - 1;
				found_free_cluster = 1;
				goto checks;
			}
			if (unlikely(--latency_ration < 0)) {
				cond_resched();
				latency_ration = LATENCY_LIMIT;
			}
		}

		offset = si->lowest_bit;
		last_in_cluster = offset + SWAPFILE_CLUSTER - 1;

		 
		for (; last_in_cluster < scan_base; offset++) {
			if (si->swap_map[offset])
				last_in_cluster = offset + SWAPFILE_CLUSTER;
			else if (offset == last_in_cluster) {
				spin_lock(&swap_lock);
				offset -= SWAPFILE_CLUSTER - 1;
				si->cluster_next = offset;
				si->cluster_nr = SWAPFILE_CLUSTER - 1;
				found_free_cluster = 1;
				goto checks;
			}
			if (unlikely(--latency_ration < 0)) {
				cond_resched();
				latency_ration = LATENCY_LIMIT;
			}
		}

		offset = scan_base;
		spin_lock(&swap_lock);
		si->cluster_nr = SWAPFILE_CLUSTER - 1;
		si->lowest_alloc = 0;
	}

checks:
	if (!(si->flags & SWP_WRITEOK))
		goto no_page;
	if (!si->highest_bit)
		goto no_page;
	if (offset > si->highest_bit)
		scan_base = offset = si->lowest_bit;

	 
	if (vm_swap_full() && si->swap_map[offset] == SWAP_HAS_CACHE) {
		int swap_was_freed;
		spin_unlock(&swap_lock);
		swap_was_freed = __try_to_reclaim_swap(si, offset);
		spin_lock(&swap_lock);
		 
		if (swap_was_freed)
			goto checks;
		goto scan;  
	}

	if (si->swap_map[offset])
		goto scan;

	if (offset == si->lowest_bit)
		si->lowest_bit++;
	if (offset == si->highest_bit)
		si->highest_bit--;
	si->inuse_pages++;
	if (si->inuse_pages == si->pages) {
		si->lowest_bit = si->max;
		si->highest_bit = 0;
	}
	si->swap_map[offset] = usage;
	si->cluster_next = offset + 1;
	si->flags -= SWP_SCANNING;

	if (si->lowest_alloc) {
		 
		if (found_free_cluster) {
			 
			if (offset < si->highest_alloc &&
			    si->lowest_alloc <= last_in_cluster)
				last_in_cluster = si->lowest_alloc - 1;
			si->flags |= SWP_DISCARDING;
			spin_unlock(&swap_lock);

			if (offset < last_in_cluster)
				discard_swap_cluster(si, offset,
					last_in_cluster - offset + 1);

			spin_lock(&swap_lock);
			si->lowest_alloc = 0;
			si->flags &= ~SWP_DISCARDING;

			smp_mb();	 
			wake_up_bit(&si->flags, ilog2(SWP_DISCARDING));

		} else if (si->flags & SWP_DISCARDING) {
			 
			spin_unlock(&swap_lock);
			wait_on_bit(&si->flags, ilog2(SWP_DISCARDING),
				wait_for_discard, TASK_UNINTERRUPTIBLE);
			spin_lock(&swap_lock);
		} else {
			 
			if (offset < si->lowest_alloc)
				si->lowest_alloc = offset;
			if (offset > si->highest_alloc)
				si->highest_alloc = offset;
		}
	}
	return offset;

scan:
	spin_unlock(&swap_lock);
	while (++offset <= si->highest_bit) {
		if (!si->swap_map[offset]) {
			spin_lock(&swap_lock);
			goto checks;
		}
		if (vm_swap_full() && si->swap_map[offset] == SWAP_HAS_CACHE) {
			spin_lock(&swap_lock);
			goto checks;
		}
		if (unlikely(--latency_ration < 0)) {
			cond_resched();
			latency_ration = LATENCY_LIMIT;
		}
	}
	offset = si->lowest_bit;
	while (++offset < scan_base) {
		if (!si->swap_map[offset]) {
			spin_lock(&swap_lock);
			goto checks;
		}
		if (vm_swap_full() && si->swap_map[offset] == SWAP_HAS_CACHE) {
			spin_lock(&swap_lock);
			goto checks;
		}
		if (unlikely(--latency_ration < 0)) {
			cond_resched();
			latency_ration = LATENCY_LIMIT;
		}
	}
	spin_lock(&swap_lock);

no_page:
	si->flags -= SWP_SCANNING;
	return 0;
}
