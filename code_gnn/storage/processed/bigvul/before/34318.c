struct inode *btrfs_lookup_dentry(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode;
	struct btrfs_root *root = BTRFS_I(dir)->root;
	struct btrfs_root *sub_root = root;
	struct btrfs_key location;
	int index;
	int ret = 0;

	if (dentry->d_name.len > BTRFS_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

	if (unlikely(d_need_lookup(dentry))) {
		memcpy(&location, dentry->d_fsdata, sizeof(struct btrfs_key));
		kfree(dentry->d_fsdata);
		dentry->d_fsdata = NULL;
		 
		d_drop(dentry);
	} else {
		ret = btrfs_inode_by_name(dir, dentry, &location);
	}

	if (ret < 0)
		return ERR_PTR(ret);

	if (location.objectid == 0)
		return NULL;

	if (location.type == BTRFS_INODE_ITEM_KEY) {
		inode = btrfs_iget(dir->i_sb, &location, root, NULL);
		return inode;
	}

	BUG_ON(location.type != BTRFS_ROOT_ITEM_KEY);

	index = srcu_read_lock(&root->fs_info->subvol_srcu);
	ret = fixup_tree_root_location(root, dir, dentry,
				       &location, &sub_root);
	if (ret < 0) {
		if (ret != -ENOENT)
			inode = ERR_PTR(ret);
		else
			inode = new_simple_dir(dir->i_sb, &location, sub_root);
	} else {
		inode = btrfs_iget(dir->i_sb, &location, sub_root, NULL);
	}
	srcu_read_unlock(&root->fs_info->subvol_srcu, index);

	if (!IS_ERR(inode) && root != sub_root) {
		down_read(&root->fs_info->cleanup_work_sem);
		if (!(inode->i_sb->s_flags & MS_RDONLY))
			ret = btrfs_orphan_cleanup(sub_root);
		up_read(&root->fs_info->cleanup_work_sem);
		if (ret)
			inode = ERR_PTR(ret);
	}

	return inode;
}