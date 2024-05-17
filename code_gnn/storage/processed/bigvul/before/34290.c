void btrfs_destroy_inode(struct inode *inode)
{
	struct btrfs_ordered_extent *ordered;
	struct btrfs_root *root = BTRFS_I(inode)->root;

	WARN_ON(!hlist_empty(&inode->i_dentry));
	WARN_ON(inode->i_data.nrpages);
	WARN_ON(BTRFS_I(inode)->outstanding_extents);
	WARN_ON(BTRFS_I(inode)->reserved_extents);
	WARN_ON(BTRFS_I(inode)->delalloc_bytes);
	WARN_ON(BTRFS_I(inode)->csum_bytes);

	 
	if (!root)
		goto free;

	 
	smp_mb();
	if (!list_empty(&BTRFS_I(inode)->ordered_operations)) {
		spin_lock(&root->fs_info->ordered_extent_lock);
		list_del_init(&BTRFS_I(inode)->ordered_operations);
		spin_unlock(&root->fs_info->ordered_extent_lock);
	}

	if (test_bit(BTRFS_INODE_HAS_ORPHAN_ITEM,
		     &BTRFS_I(inode)->runtime_flags)) {
		printk(KERN_INFO "BTRFS: inode %llu still on the orphan list\n",
		       (unsigned long long)btrfs_ino(inode));
		atomic_dec(&root->orphan_inodes);
	}

	while (1) {
		ordered = btrfs_lookup_first_ordered_extent(inode, (u64)-1);
		if (!ordered)
			break;
		else {
			printk(KERN_ERR "btrfs found ordered "
			       "extent %llu %llu on inode cleanup\n",
			       (unsigned long long)ordered->file_offset,
			       (unsigned long long)ordered->len);
			btrfs_remove_ordered_extent(inode, ordered);
			btrfs_put_ordered_extent(ordered);
			btrfs_put_ordered_extent(ordered);
		}
	}
	inode_tree_del(inode);
	btrfs_drop_extent_cache(inode, 0, (u64)-1, 0);
free:
	btrfs_remove_delayed_node(inode);
	call_rcu(&inode->i_rcu, btrfs_i_callback);
}