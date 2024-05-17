static int btrfs_setsize(struct inode *inode, struct iattr *attr)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_trans_handle *trans;
	loff_t oldsize = i_size_read(inode);
	loff_t newsize = attr->ia_size;
	int mask = attr->ia_valid;
	int ret;

	 
	if (newsize != oldsize) {
		inode_inc_iversion(inode);
		if (!(mask & (ATTR_CTIME | ATTR_MTIME)))
			inode->i_ctime = inode->i_mtime =
				current_fs_time(inode->i_sb);
	}

	if (newsize > oldsize) {
		truncate_pagecache(inode, newsize);
		 
		wait_for_snapshot_creation(root);
		ret = btrfs_cont_expand(inode, oldsize, newsize);
		if (ret) {
			btrfs_end_write_no_snapshoting(root);
			return ret;
		}

		trans = btrfs_start_transaction(root, 1);
		if (IS_ERR(trans)) {
			btrfs_end_write_no_snapshoting(root);
			return PTR_ERR(trans);
		}

		i_size_write(inode, newsize);
		btrfs_ordered_update_i_size(inode, i_size_read(inode), NULL);
		ret = btrfs_update_inode(trans, root, inode);
		btrfs_end_write_no_snapshoting(root);
		btrfs_end_transaction(trans, root);
	} else {

		 
		if (newsize == 0)
			set_bit(BTRFS_INODE_ORDERED_DATA_CLOSE,
				&BTRFS_I(inode)->runtime_flags);

		 
		trans = btrfs_start_transaction(root, 2);
		if (IS_ERR(trans))
			return PTR_ERR(trans);

		 
		ret = btrfs_orphan_add(trans, inode);
		btrfs_end_transaction(trans, root);
		if (ret)
			return ret;

		 
		truncate_setsize(inode, newsize);

		 
		btrfs_inode_block_unlocked_dio(inode);
		inode_dio_wait(inode);
		btrfs_inode_resume_unlocked_dio(inode);

		ret = btrfs_truncate(inode);
		if (ret && inode->i_nlink) {
			int err;

			 
			trans = btrfs_join_transaction(root);
			if (IS_ERR(trans)) {
				btrfs_orphan_del(NULL, inode);
				return ret;
			}
			i_size_write(inode, BTRFS_I(inode)->disk_i_size);
			err = btrfs_orphan_del(trans, inode);
			if (err)
				btrfs_abort_transaction(trans, root, err);
			btrfs_end_transaction(trans, root);
		}
	}

	return ret;
}
