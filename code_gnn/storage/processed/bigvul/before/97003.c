int hugetlb_reserve_pages(struct inode *inode,
					long from, long to,
					struct vm_area_struct *vma,
					vm_flags_t vm_flags)
{
	long ret, chg;
	struct hstate *h = hstate_inode(inode);
	struct hugepage_subpool *spool = subpool_inode(inode);
	struct resv_map *resv_map;
	long gbl_reserve;

	 
	if (from > to) {
		VM_WARN(1, "%s called with a negative range\n", __func__);
		return -EINVAL;
	}

	 
	if (vm_flags & VM_NORESERVE)
		return 0;

	 
	if (!vma || vma->vm_flags & VM_MAYSHARE) {
		resv_map = inode_resv_map(inode);

		chg = region_chg(resv_map, from, to);

	} else {
		resv_map = resv_map_alloc();
		if (!resv_map)
			return -ENOMEM;

		chg = to - from;

		set_vma_resv_map(vma, resv_map);
		set_vma_resv_flags(vma, HPAGE_RESV_OWNER);
	}

	if (chg < 0) {
		ret = chg;
		goto out_err;
	}

	 
	gbl_reserve = hugepage_subpool_get_pages(spool, chg);
	if (gbl_reserve < 0) {
		ret = -ENOSPC;
		goto out_err;
	}

	 
	ret = hugetlb_acct_memory(h, gbl_reserve);
	if (ret < 0) {
		 
		(void)hugepage_subpool_put_pages(spool, chg);
		goto out_err;
	}

	 
	if (!vma || vma->vm_flags & VM_MAYSHARE) {
		long add = region_add(resv_map, from, to);

		if (unlikely(chg > add)) {
			 
			long rsv_adjust;

			rsv_adjust = hugepage_subpool_put_pages(spool,
								chg - add);
			hugetlb_acct_memory(h, -rsv_adjust);
		}
	}
	return 0;
out_err:
	if (!vma || vma->vm_flags & VM_MAYSHARE)
		 
		if (chg >= 0)
			region_abort(resv_map, from, to);
	if (vma && is_vma_resv_set(vma, HPAGE_RESV_OWNER))
		kref_put(&resv_map->refs, resv_map_release);
	return ret;
}
