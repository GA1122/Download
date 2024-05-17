static void purge_persistent_gnt(struct xen_blkif_ring *ring)
{
	struct persistent_gnt *persistent_gnt;
	struct rb_node *n;
	unsigned int num_clean, total;
	bool scan_used = false, clean_used = false;
	struct rb_root *root;

	if (ring->persistent_gnt_c < xen_blkif_max_pgrants ||
	    (ring->persistent_gnt_c == xen_blkif_max_pgrants &&
	    !ring->blkif->vbd.overflow_max_grants)) {
		goto out;
	}

	if (work_busy(&ring->persistent_purge_work)) {
		pr_alert_ratelimited("Scheduled work from previous purge is still busy, cannot purge list\n");
		goto out;
	}

	num_clean = (xen_blkif_max_pgrants / 100) * LRU_PERCENT_CLEAN;
	num_clean = ring->persistent_gnt_c - xen_blkif_max_pgrants + num_clean;
	num_clean = min(ring->persistent_gnt_c, num_clean);
	if ((num_clean == 0) ||
	    (num_clean > (ring->persistent_gnt_c - atomic_read(&ring->persistent_gnt_in_use))))
		goto out;

	 

	total = num_clean;

	pr_debug("Going to purge %u persistent grants\n", num_clean);

	BUG_ON(!list_empty(&ring->persistent_purge_list));
	root = &ring->persistent_gnts;
purge_list:
	foreach_grant_safe(persistent_gnt, n, root, node) {
		BUG_ON(persistent_gnt->handle ==
			BLKBACK_INVALID_HANDLE);

		if (clean_used) {
			clear_bit(PERSISTENT_GNT_WAS_ACTIVE, persistent_gnt->flags);
			continue;
		}

		if (test_bit(PERSISTENT_GNT_ACTIVE, persistent_gnt->flags))
			continue;
		if (!scan_used &&
		    (test_bit(PERSISTENT_GNT_WAS_ACTIVE, persistent_gnt->flags)))
			continue;

		rb_erase(&persistent_gnt->node, root);
		list_add(&persistent_gnt->remove_node,
			 &ring->persistent_purge_list);
		if (--num_clean == 0)
			goto finished;
	}
	 
	if (!scan_used && !clean_used) {
		pr_debug("Still missing %u purged frames\n", num_clean);
		scan_used = true;
		goto purge_list;
	}
finished:
	if (!clean_used) {
		pr_debug("Finished scanning for grants to clean, removing used flag\n");
		clean_used = true;
		goto purge_list;
	}

	ring->persistent_gnt_c -= (total - num_clean);
	ring->blkif->vbd.overflow_max_grants = 0;

	 
	schedule_work(&ring->persistent_purge_work);
	pr_debug("Purged %u/%u\n", (total - num_clean), total);

out:
	return;
}
