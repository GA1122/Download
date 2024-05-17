static void destroy_inodecache(void)
{
	 
	rcu_barrier();
	kmem_cache_destroy(f2fs_inode_cachep);
}
