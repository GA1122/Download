static void btrfs_read_locked_inode(struct inode *inode)
{
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_inode_item *inode_item;
	struct btrfs_timespec *tspec;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_key location;
	int maybe_acls;
	u32 rdev;
	int ret;
	bool filled = false;

	ret = btrfs_fill_inode(inode, &rdev);
	if (!ret)
		filled = true;

	path = btrfs_alloc_path();
	if (!path)
		goto make_bad;

	path->leave_spinning = 1;
	memcpy(&location, &BTRFS_I(inode)->location, sizeof(location));

	ret = btrfs_lookup_inode(NULL, root, path, &location, 0);
	if (ret)
		goto make_bad;

	leaf = path->nodes[0];

	if (filled)
		goto cache_acl;

	inode_item = btrfs_item_ptr(leaf, path->slots[0],
				    struct btrfs_inode_item);
	inode->i_mode = btrfs_inode_mode(leaf, inode_item);
	set_nlink(inode, btrfs_inode_nlink(leaf, inode_item));
	i_uid_write(inode, btrfs_inode_uid(leaf, inode_item));
	i_gid_write(inode, btrfs_inode_gid(leaf, inode_item));
	btrfs_i_size_write(inode, btrfs_inode_size(leaf, inode_item));

	tspec = btrfs_inode_atime(inode_item);
	inode->i_atime.tv_sec = btrfs_timespec_sec(leaf, tspec);
	inode->i_atime.tv_nsec = btrfs_timespec_nsec(leaf, tspec);

	tspec = btrfs_inode_mtime(inode_item);
	inode->i_mtime.tv_sec = btrfs_timespec_sec(leaf, tspec);
	inode->i_mtime.tv_nsec = btrfs_timespec_nsec(leaf, tspec);

	tspec = btrfs_inode_ctime(inode_item);
	inode->i_ctime.tv_sec = btrfs_timespec_sec(leaf, tspec);
	inode->i_ctime.tv_nsec = btrfs_timespec_nsec(leaf, tspec);

	inode_set_bytes(inode, btrfs_inode_nbytes(leaf, inode_item));
	BTRFS_I(inode)->generation = btrfs_inode_generation(leaf, inode_item);
	BTRFS_I(inode)->last_trans = btrfs_inode_transid(leaf, inode_item);

	 
	if (BTRFS_I(inode)->last_trans == root->fs_info->generation)
		set_bit(BTRFS_INODE_NEEDS_FULL_SYNC,
			&BTRFS_I(inode)->runtime_flags);

	inode->i_version = btrfs_inode_sequence(leaf, inode_item);
	inode->i_generation = BTRFS_I(inode)->generation;
	inode->i_rdev = 0;
	rdev = btrfs_inode_rdev(leaf, inode_item);

	BTRFS_I(inode)->index_cnt = (u64)-1;
	BTRFS_I(inode)->flags = btrfs_inode_flags(leaf, inode_item);
cache_acl:
	 
	maybe_acls = acls_after_inode_item(leaf, path->slots[0],
					   btrfs_ino(inode));
	if (!maybe_acls)
		cache_no_acl(inode);

	btrfs_free_path(path);

	switch (inode->i_mode & S_IFMT) {
	case S_IFREG:
		inode->i_mapping->a_ops = &btrfs_aops;
		inode->i_mapping->backing_dev_info = &root->fs_info->bdi;
		BTRFS_I(inode)->io_tree.ops = &btrfs_extent_io_ops;
		inode->i_fop = &btrfs_file_operations;
		inode->i_op = &btrfs_file_inode_operations;
		break;
	case S_IFDIR:
		inode->i_fop = &btrfs_dir_file_operations;
		if (root == root->fs_info->tree_root)
			inode->i_op = &btrfs_dir_ro_inode_operations;
		else
			inode->i_op = &btrfs_dir_inode_operations;
		break;
	case S_IFLNK:
		inode->i_op = &btrfs_symlink_inode_operations;
		inode->i_mapping->a_ops = &btrfs_symlink_aops;
		inode->i_mapping->backing_dev_info = &root->fs_info->bdi;
		break;
	default:
		inode->i_op = &btrfs_special_inode_operations;
		init_special_inode(inode, inode->i_mode, rdev);
		break;
	}

	btrfs_update_iflags(inode);
	return;

make_bad:
	btrfs_free_path(path);
	make_bad_inode(inode);
}