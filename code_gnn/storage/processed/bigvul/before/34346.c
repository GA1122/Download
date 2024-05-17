static int btrfs_setsize(struct inode *inode, loff_t newsize)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_trans_handle *trans;
	loff_t oldsize = i_size_read(inode);
	int ret;

	if (newsize == oldsize)
		return 0;

	if (newsize > oldsize) {
		truncate_pagecache(inode, oldsize, newsize);
		ret = btrfs_cont_expand(inode, oldsize, newsize);
		if (ret)
			return ret;

		trans = btrfs_start_transaction(root, 1);
		if (IS_ERR(trans))
			return PTR_ERR(trans);

		i_size_write(inode, newsize);
		btrfs_ordered_update_i_size(inode, i_size_read(inode), NULL);
		ret = btrfs_update_inode(trans, root, inode);
		btrfs_end_transaction(trans, root);
	} else {

		 
		if (newsize == 0)
			set_bit(BTRFS_INODE_ORDERED_DATA_CLOSE,
				&BTRFS_I(inode)->runtime_flags);

		 
		truncate_setsize(inode, newsize);
		ret = btrfs_truncate(inode);
	}

	return ret;
}