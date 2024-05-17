static noinline int can_nocow_odirect(struct btrfs_trans_handle *trans,
				      struct inode *inode, u64 offset, u64 len)
{
	struct btrfs_path *path;
	int ret;
	struct extent_buffer *leaf;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_file_extent_item *fi;
	struct btrfs_key key;
	u64 disk_bytenr;
	u64 backref_offset;
	u64 extent_end;
	u64 num_bytes;
	int slot;
	int found_type;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	ret = btrfs_lookup_file_extent(trans, root, path, btrfs_ino(inode),
				       offset, 0);
	if (ret < 0)
		goto out;

	slot = path->slots[0];
	if (ret == 1) {
		if (slot == 0) {
			 
			ret = 0;
			goto out;
		}
		slot--;
	}
	ret = 0;
	leaf = path->nodes[0];
	btrfs_item_key_to_cpu(leaf, &key, slot);
	if (key.objectid != btrfs_ino(inode) ||
	    key.type != BTRFS_EXTENT_DATA_KEY) {
		 
		goto out;
	}

	if (key.offset > offset) {
		 
		goto out;
	}

	fi = btrfs_item_ptr(leaf, slot, struct btrfs_file_extent_item);
	found_type = btrfs_file_extent_type(leaf, fi);
	if (found_type != BTRFS_FILE_EXTENT_REG &&
	    found_type != BTRFS_FILE_EXTENT_PREALLOC) {
		 
		goto out;
	}
	disk_bytenr = btrfs_file_extent_disk_bytenr(leaf, fi);
	backref_offset = btrfs_file_extent_offset(leaf, fi);

	extent_end = key.offset + btrfs_file_extent_num_bytes(leaf, fi);
	if (extent_end < offset + len) {
		 
		goto out;
	}

	if (btrfs_extent_readonly(root, disk_bytenr))
		goto out;

	 
	if (btrfs_cross_ref_exist(trans, root, btrfs_ino(inode),
				  key.offset - backref_offset, disk_bytenr))
		goto out;

	 
	disk_bytenr += backref_offset;
	disk_bytenr += offset - key.offset;
	num_bytes = min(offset + len, extent_end) - offset;
	if (csum_exist_in_range(root, disk_bytenr, num_bytes))
				goto out;
	 
	ret = 1;
out:
	btrfs_free_path(path);
	return ret;
}
