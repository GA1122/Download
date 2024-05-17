static struct page *shmem_alloc_page(gfp_t gfp,
			struct shmem_inode_info *info, pgoff_t index)
{
	struct vm_area_struct pvma;
	struct page *page;

	 
	pvma.vm_start = 0;
	 
	pvma.vm_pgoff = index + info->vfs_inode.i_ino;
	pvma.vm_ops = NULL;
	pvma.vm_policy = mpol_shared_policy_lookup(&info->policy, index);

	page = alloc_page_vma(gfp, &pvma, 0);

	 
	mpol_cond_put(pvma.vm_policy);

	return page;
}
