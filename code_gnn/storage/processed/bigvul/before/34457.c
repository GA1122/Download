static noinline int search_ioctl(struct inode *inode,
				 struct btrfs_ioctl_search_args *args)
{
	struct btrfs_root *root;
	struct btrfs_key key;
	struct btrfs_key max_key;
	struct btrfs_path *path;
	struct btrfs_ioctl_search_key *sk = &args->key;
	struct btrfs_fs_info *info = BTRFS_I(inode)->root->fs_info;
	int ret;
	int num_found = 0;
	unsigned long sk_offset = 0;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	if (sk->tree_id == 0) {
		 
		root = BTRFS_I(inode)->root;
	} else {
		key.objectid = sk->tree_id;
		key.type = BTRFS_ROOT_ITEM_KEY;
		key.offset = (u64)-1;
		root = btrfs_read_fs_root_no_name(info, &key);
		if (IS_ERR(root)) {
			printk(KERN_ERR "could not find root %llu\n",
			       sk->tree_id);
			btrfs_free_path(path);
			return -ENOENT;
		}
	}

	key.objectid = sk->min_objectid;
	key.type = sk->min_type;
	key.offset = sk->min_offset;

	max_key.objectid = sk->max_objectid;
	max_key.type = sk->max_type;
	max_key.offset = sk->max_offset;

	path->keep_locks = 1;

	while(1) {
		ret = btrfs_search_forward(root, &key, &max_key, path, 0,
					   sk->min_transid);
		if (ret != 0) {
			if (ret > 0)
				ret = 0;
			goto err;
		}
		ret = copy_to_sk(root, path, &key, sk, args->buf,
				 &sk_offset, &num_found);
		btrfs_release_path(path);
		if (ret || num_found >= sk->nr_items)
			break;

	}
	ret = 0;
err:
	sk->nr_items = num_found;
	btrfs_free_path(path);
	return ret;
}