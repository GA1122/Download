static int f2fs_drop_inode(struct inode *inode)
{
	int ret;
	 
	if ((!inode_unhashed(inode) && inode->i_state & I_SYNC)) {
		if (!inode->i_nlink && !is_bad_inode(inode)) {
			 
			atomic_inc(&inode->i_count);
			spin_unlock(&inode->i_lock);

			 
			if (f2fs_is_atomic_file(inode))
				drop_inmem_pages(inode);

			 
			f2fs_destroy_extent_node(inode);

			sb_start_intwrite(inode->i_sb);
			f2fs_i_size_write(inode, 0);

			if (F2FS_HAS_BLOCKS(inode))
				f2fs_truncate(inode);

			sb_end_intwrite(inode->i_sb);

			fscrypt_put_encryption_info(inode, NULL);
			spin_lock(&inode->i_lock);
			atomic_dec(&inode->i_count);
		}
		trace_f2fs_drop_inode(inode, 0);
		return 0;
	}
	ret = generic_drop_inode(inode);
	trace_f2fs_drop_inode(inode, ret);
	return ret;
}
