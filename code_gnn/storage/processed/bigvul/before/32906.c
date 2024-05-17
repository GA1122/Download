static void destroy_inodecache(void)
{
	 
	rcu_barrier();
	kmem_cache_destroy(ext3_inode_cachep);
}
