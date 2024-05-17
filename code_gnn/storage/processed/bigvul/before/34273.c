static struct btrfs_trans_handle *__unlink_start_trans(struct inode *dir,
						       struct dentry *dentry)
{
	struct btrfs_trans_handle *trans;
	struct btrfs_root *root = BTRFS_I(dir)->root;
	struct btrfs_path *path;
	struct btrfs_dir_item *di;
	struct inode *inode = dentry->d_inode;
	u64 index;
	int check_link = 1;
	int err = -ENOSPC;
	int ret;
	u64 ino = btrfs_ino(inode);
	u64 dir_ino = btrfs_ino(dir);

	 
	trans = btrfs_start_transaction(root, 8);
	if (!IS_ERR(trans) || PTR_ERR(trans) != -ENOSPC)
		return trans;

	if (ino == BTRFS_EMPTY_SUBVOL_DIR_OBJECTID)
		return ERR_PTR(-ENOSPC);

	 
	if (S_ISDIR(inode->i_mode) && atomic_read(&inode->i_count) > 1)
		return ERR_PTR(-ENOSPC);

	if (atomic_read(&inode->i_count) > 2)
		return ERR_PTR(-ENOSPC);

	if (xchg(&root->fs_info->enospc_unlink, 1))
		return ERR_PTR(-ENOSPC);

	path = btrfs_alloc_path();
	if (!path) {
		root->fs_info->enospc_unlink = 0;
		return ERR_PTR(-ENOMEM);
	}

	 
	trans = btrfs_start_transaction(root, 1);
	if (IS_ERR(trans)) {
		btrfs_free_path(path);
		root->fs_info->enospc_unlink = 0;
		return trans;
	}

	path->skip_locking = 1;
	path->search_commit_root = 1;

	ret = btrfs_lookup_inode(trans, root, path,
				&BTRFS_I(dir)->location, 0);
	if (ret < 0) {
		err = ret;
		goto out;
	}
	if (ret == 0) {
		if (check_path_shared(root, path))
			goto out;
	} else {
		check_link = 0;
	}
	btrfs_release_path(path);

	ret = btrfs_lookup_inode(trans, root, path,
				&BTRFS_I(inode)->location, 0);
	if (ret < 0) {
		err = ret;
		goto out;
	}
	if (ret == 0) {
		if (check_path_shared(root, path))
			goto out;
	} else {
		check_link = 0;
	}
	btrfs_release_path(path);

	if (ret == 0 && S_ISREG(inode->i_mode)) {
		ret = btrfs_lookup_file_extent(trans, root, path,
					       ino, (u64)-1, 0);
		if (ret < 0) {
			err = ret;
			goto out;
		}
		BUG_ON(ret == 0);  
		if (check_path_shared(root, path))
			goto out;
		btrfs_release_path(path);
	}

	if (!check_link) {
		err = 0;
		goto out;
	}

	di = btrfs_lookup_dir_item(trans, root, path, dir_ino,
				dentry->d_name.name, dentry->d_name.len, 0);
	if (IS_ERR(di)) {
		err = PTR_ERR(di);
		goto out;
	}
	if (di) {
		if (check_path_shared(root, path))
			goto out;
	} else {
		err = 0;
		goto out;
	}
	btrfs_release_path(path);

	ret = btrfs_get_inode_ref_index(trans, root, path, dentry->d_name.name,
					dentry->d_name.len, ino, dir_ino, 0,
					&index);
	if (ret) {
		err = ret;
		goto out;
	}

	if (check_path_shared(root, path))
		goto out;

	btrfs_release_path(path);

	 
	di = btrfs_lookup_dir_index_item(trans, root, path, dir_ino, index,
				dentry->d_name.name, dentry->d_name.len, 0);
	if (IS_ERR(di)) {
		err = PTR_ERR(di);
		goto out;
	}
	BUG_ON(ret == -ENOENT);
	if (check_path_shared(root, path))
		goto out;

	err = 0;
out:
	btrfs_free_path(path);
	 
	if (!err)
		err = btrfs_block_rsv_migrate(trans->block_rsv,
				&root->fs_info->global_block_rsv,
				trans->bytes_reserved);

	if (err) {
		btrfs_end_transaction(trans, root);
		root->fs_info->enospc_unlink = 0;
		return ERR_PTR(err);
	}

	trans->block_rsv = &root->fs_info->global_block_rsv;
	return trans;
}
