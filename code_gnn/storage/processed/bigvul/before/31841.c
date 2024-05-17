static int xen_blkbk_map(struct xen_blkif *blkif,
			 struct grant_page *pages[],
			 int num, bool ro)
{
	struct gnttab_map_grant_ref map[BLKIF_MAX_SEGMENTS_PER_REQUEST];
	struct page *pages_to_gnt[BLKIF_MAX_SEGMENTS_PER_REQUEST];
	struct persistent_gnt *persistent_gnt = NULL;
	phys_addr_t addr = 0;
	int i, seg_idx, new_map_idx;
	int segs_to_map = 0;
	int ret = 0;
	int last_map = 0, map_until = 0;
	int use_persistent_gnts;

	use_persistent_gnts = (blkif->vbd.feature_gnt_persistent);

	 
again:
	for (i = map_until; i < num; i++) {
		uint32_t flags;

		if (use_persistent_gnts)
			persistent_gnt = get_persistent_gnt(
				blkif,
				pages[i]->gref);

		if (persistent_gnt) {
			 
			pages[i]->page = persistent_gnt->page;
			pages[i]->persistent_gnt = persistent_gnt;
		} else {
			if (get_free_page(blkif, &pages[i]->page))
				goto out_of_memory;
			addr = vaddr(pages[i]->page);
			pages_to_gnt[segs_to_map] = pages[i]->page;
			pages[i]->persistent_gnt = NULL;
			flags = GNTMAP_host_map;
			if (!use_persistent_gnts && ro)
				flags |= GNTMAP_readonly;
			gnttab_set_map_op(&map[segs_to_map++], addr,
					  flags, pages[i]->gref,
					  blkif->domid);
		}
		map_until = i + 1;
		if (segs_to_map == BLKIF_MAX_SEGMENTS_PER_REQUEST)
			break;
	}

	if (segs_to_map) {
		ret = gnttab_map_refs(map, NULL, pages_to_gnt, segs_to_map);
		BUG_ON(ret);
	}

	 
	for (seg_idx = last_map, new_map_idx = 0; seg_idx < map_until; seg_idx++) {
		if (!pages[seg_idx]->persistent_gnt) {
			 
			BUG_ON(new_map_idx >= segs_to_map);
			if (unlikely(map[new_map_idx].status != 0)) {
				pr_debug(DRV_PFX "invalid buffer -- could not remap it\n");
				pages[seg_idx]->handle = BLKBACK_INVALID_HANDLE;
				ret |= 1;
				goto next;
			}
			pages[seg_idx]->handle = map[new_map_idx].handle;
		} else {
			continue;
		}
		if (use_persistent_gnts &&
		    blkif->persistent_gnt_c < xen_blkif_max_pgrants) {
			 
			persistent_gnt = kmalloc(sizeof(struct persistent_gnt),
				                 GFP_KERNEL);
			if (!persistent_gnt) {
				 
				goto next;
			}
			persistent_gnt->gnt = map[new_map_idx].ref;
			persistent_gnt->handle = map[new_map_idx].handle;
			persistent_gnt->page = pages[seg_idx]->page;
			if (add_persistent_gnt(blkif,
			                       persistent_gnt)) {
				kfree(persistent_gnt);
				persistent_gnt = NULL;
				goto next;
			}
			pages[seg_idx]->persistent_gnt = persistent_gnt;
			pr_debug(DRV_PFX " grant %u added to the tree of persistent grants, using %u/%u\n",
				 persistent_gnt->gnt, blkif->persistent_gnt_c,
				 xen_blkif_max_pgrants);
			goto next;
		}
		if (use_persistent_gnts && !blkif->vbd.overflow_max_grants) {
			blkif->vbd.overflow_max_grants = 1;
			pr_debug(DRV_PFX " domain %u, device %#x is using maximum number of persistent grants\n",
			         blkif->domid, blkif->vbd.handle);
		}
		 
next:
		new_map_idx++;
	}
	segs_to_map = 0;
	last_map = map_until;
	if (map_until != num)
		goto again;

	return ret;

out_of_memory:
	pr_alert(DRV_PFX "%s: out of memory\n", __func__);
	put_free_pages(blkif, pages_to_gnt, segs_to_map);
	return -ENOMEM;
}
