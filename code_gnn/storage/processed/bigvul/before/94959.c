static void ext2_i_callback(struct rcu_head *head)
{
	struct inode *inode = container_of(head, struct inode, i_rcu);
	kmem_cache_free(ext2_inode_cachep, EXT2_I(inode));
}
