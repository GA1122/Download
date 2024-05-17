ssize_t btrfs_listxattr(struct dentry *dentry, char *buffer, size_t size)
{
	struct btrfs_key key, found_key;
	struct inode *inode = dentry->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_dir_item *di;
	int ret = 0, slot;
	size_t total_size = 0, size_left = size;
	unsigned long name_ptr;
	size_t name_len;

	 
	key.objectid = btrfs_ino(inode);
	key.type = BTRFS_XATTR_ITEM_KEY;
	key.offset = 0;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
	path->reada = 2;

	 
	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto err;

	while (1) {
		leaf = path->nodes[0];
		slot = path->slots[0];

		 
		if (slot >= btrfs_header_nritems(leaf)) {
			 
			ret = btrfs_next_leaf(root, path);
			if (ret < 0)
				goto err;
			else if (ret > 0)
				break;
			continue;
		}

		btrfs_item_key_to_cpu(leaf, &found_key, slot);

		 
		if (found_key.objectid != key.objectid)
			break;
		if (found_key.type != BTRFS_XATTR_ITEM_KEY)
			break;

		di = btrfs_item_ptr(leaf, slot, struct btrfs_dir_item);
		if (verify_dir_item(root, leaf, di))
			goto next;

		name_len = btrfs_dir_name_len(leaf, di);
		total_size += name_len + 1;

		 
		if (!size)
			goto next;

		if (!buffer || (name_len + 1) > size_left) {
			ret = -ERANGE;
			goto err;
		}

		name_ptr = (unsigned long)(di + 1);
		read_extent_buffer(leaf, buffer, name_ptr, name_len);
		buffer[name_len] = '\0';

		size_left -= name_len + 1;
		buffer += name_len + 1;
next:
		path->slots[0]++;
	}
	ret = total_size;

err:
	btrfs_free_path(path);

	return ret;
}
