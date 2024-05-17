static int btrfs_create(struct inode *dir, struct dentry *dentry,
			umode_t mode, bool excl)
{
	struct btrfs_trans_handle *trans;
	struct btrfs_root *root = BTRFS_I(dir)->root;
	struct inode *inode = NULL;
	int drop_inode_on_err = 0;
	int err;
	u64 objectid;
	u64 index = 0;

	 
	trans = btrfs_start_transaction(root, 5);
	if (IS_ERR(trans))
		return PTR_ERR(trans);

	err = btrfs_find_free_ino(root, &objectid);
	if (err)
		goto out_unlock;

	inode = btrfs_new_inode(trans, root, dir, dentry->d_name.name,
				dentry->d_name.len, btrfs_ino(dir), objectid,
				mode, &index);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out_unlock;
	}
	drop_inode_on_err = 1;

	err = btrfs_init_inode_security(trans, inode, dir, &dentry->d_name);
	if (err)
		goto out_unlock;

	err = btrfs_update_inode(trans, root, inode);
	if (err)
		goto out_unlock;

	 
	inode->i_fop = &btrfs_file_operations;
	inode->i_op = &btrfs_file_inode_operations;

	err = btrfs_add_nondir(trans, dir, dentry, inode, 0, index);
	if (err)
		goto out_unlock;

	inode->i_mapping->a_ops = &btrfs_aops;
	inode->i_mapping->backing_dev_info = &root->fs_info->bdi;
	BTRFS_I(inode)->io_tree.ops = &btrfs_extent_io_ops;
	d_instantiate(dentry, inode);

out_unlock:
	btrfs_end_transaction(trans, root);
	if (err && drop_inode_on_err) {
		inode_dec_link_count(inode);
		iput(inode);
	}
	btrfs_btree_balance_dirty(root);
	return err;
}