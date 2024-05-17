int btrfs_check_dir_item_collision(struct btrfs_root *root, u64 dir,
				   const char *name, int name_len)
{
	int ret;
	struct btrfs_key key;
	struct btrfs_dir_item *di;
	int data_size;
	struct extent_buffer *leaf;
	int slot;
	struct btrfs_path *path;


	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = dir;
	key.type = BTRFS_DIR_ITEM_KEY;
	key.offset = btrfs_name_hash(name, name_len);

	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);

	 
	if (ret < 0)
		goto out;

	 
	if (ret > 0) {
		ret = 0;
		goto out;
	}

	 
	di = btrfs_match_dir_item_name(root, path, name, name_len);
	if (di) {
		 
		ret = -EEXIST;
		goto out;
	}

	 
	data_size = sizeof(*di) + name_len;
	leaf = path->nodes[0];
	slot = path->slots[0];
	if (data_size + btrfs_item_size_nr(leaf, slot) +
	    sizeof(struct btrfs_item) > BTRFS_LEAF_DATA_SIZE(root)) {
		ret = -EOVERFLOW;
	} else {
		 
		ret = 0;
	}
out:
	btrfs_free_path(path);
	return ret;
}