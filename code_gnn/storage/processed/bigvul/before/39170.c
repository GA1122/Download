int __init nfs_init_writepagecache(void)
{
	nfs_wdata_cachep = kmem_cache_create("nfs_write_data",
					     sizeof(struct nfs_write_header),
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (nfs_wdata_cachep == NULL)
		return -ENOMEM;

	nfs_wdata_mempool = mempool_create_slab_pool(MIN_POOL_WRITE,
						     nfs_wdata_cachep);
	if (nfs_wdata_mempool == NULL)
		goto out_destroy_write_cache;

	nfs_cdata_cachep = kmem_cache_create("nfs_commit_data",
					     sizeof(struct nfs_commit_data),
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (nfs_cdata_cachep == NULL)
		goto out_destroy_write_mempool;

	nfs_commit_mempool = mempool_create_slab_pool(MIN_POOL_COMMIT,
						      nfs_cdata_cachep);
	if (nfs_commit_mempool == NULL)
		goto out_destroy_commit_cache;

	 
	nfs_congestion_kb = (16*int_sqrt(totalram_pages)) << (PAGE_SHIFT-10);
	if (nfs_congestion_kb > 256*1024)
		nfs_congestion_kb = 256*1024;

	return 0;

out_destroy_commit_cache:
	kmem_cache_destroy(nfs_cdata_cachep);
out_destroy_write_mempool:
	mempool_destroy(nfs_wdata_mempool);
out_destroy_write_cache:
	kmem_cache_destroy(nfs_wdata_cachep);
	return -ENOMEM;
}
