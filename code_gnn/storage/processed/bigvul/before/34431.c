static noinline int btrfs_ioctl_snap_destroy(struct file *file,
					     void __user *arg)
{
	struct dentry *parent = fdentry(file);
	struct dentry *dentry;
	struct inode *dir = parent->d_inode;
	struct inode *inode;
	struct btrfs_root *root = BTRFS_I(dir)->root;
	struct btrfs_root *dest = NULL;
	struct btrfs_ioctl_vol_args *vol_args;
	struct btrfs_trans_handle *trans;
	int namelen;
	int ret;
	int err = 0;

	vol_args = memdup_user(arg, sizeof(*vol_args));
	if (IS_ERR(vol_args))
		return PTR_ERR(vol_args);

	vol_args->name[BTRFS_PATH_NAME_MAX] = '\0';
	namelen = strlen(vol_args->name);
	if (strchr(vol_args->name, '/') ||
	    strncmp(vol_args->name, "..", namelen) == 0) {
		err = -EINVAL;
		goto out;
	}

	err = mnt_want_write_file(file);
	if (err)
		goto out;

	mutex_lock_nested(&dir->i_mutex, I_MUTEX_PARENT);
	dentry = lookup_one_len(vol_args->name, parent, namelen);
	if (IS_ERR(dentry)) {
		err = PTR_ERR(dentry);
		goto out_unlock_dir;
	}

	if (!dentry->d_inode) {
		err = -ENOENT;
		goto out_dput;
	}

	inode = dentry->d_inode;
	dest = BTRFS_I(inode)->root;
	if (!capable(CAP_SYS_ADMIN)){
		 
		err = -EPERM;
		if (!btrfs_test_opt(root, USER_SUBVOL_RM_ALLOWED))
			goto out_dput;

		 
		err = -EINVAL;
		if (root == dest)
			goto out_dput;

		err = inode_permission(inode, MAY_WRITE | MAY_EXEC);
		if (err)
			goto out_dput;

		 
		err = btrfs_may_delete(dir, dentry, 1);
		if (err)
			goto out_dput;
	}

	if (btrfs_ino(inode) != BTRFS_FIRST_FREE_OBJECTID) {
		err = -EINVAL;
		goto out_dput;
	}

	mutex_lock(&inode->i_mutex);
	err = d_invalidate(dentry);
	if (err)
		goto out_unlock;

	down_write(&root->fs_info->subvol_sem);

	err = may_destroy_subvol(dest);
	if (err)
		goto out_up_write;

	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		err = PTR_ERR(trans);
		goto out_up_write;
	}
	trans->block_rsv = &root->fs_info->global_block_rsv;

	ret = btrfs_unlink_subvol(trans, root, dir,
				dest->root_key.objectid,
				dentry->d_name.name,
				dentry->d_name.len);
	if (ret) {
		err = ret;
		btrfs_abort_transaction(trans, root, ret);
		goto out_end_trans;
	}

	btrfs_record_root_in_trans(trans, dest);

	memset(&dest->root_item.drop_progress, 0,
		sizeof(dest->root_item.drop_progress));
	dest->root_item.drop_level = 0;
	btrfs_set_root_refs(&dest->root_item, 0);

	if (!xchg(&dest->orphan_item_inserted, 1)) {
		ret = btrfs_insert_orphan_item(trans,
					root->fs_info->tree_root,
					dest->root_key.objectid);
		if (ret) {
			btrfs_abort_transaction(trans, root, ret);
			err = ret;
			goto out_end_trans;
		}
	}
out_end_trans:
	ret = btrfs_end_transaction(trans, root);
	if (ret && !err)
		err = ret;
	inode->i_flags |= S_DEAD;
out_up_write:
	up_write(&root->fs_info->subvol_sem);
out_unlock:
	mutex_unlock(&inode->i_mutex);
	if (!err) {
		shrink_dcache_sb(root->fs_info->sb);
		btrfs_invalidate_inodes(dest);
		d_delete(dentry);
	}
out_dput:
	dput(dentry);
out_unlock_dir:
	mutex_unlock(&dir->i_mutex);
	mnt_drop_write_file(file);
out:
	kfree(vol_args);
	return err;
}