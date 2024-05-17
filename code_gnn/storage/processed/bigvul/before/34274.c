static noinline int acls_after_inode_item(struct extent_buffer *leaf,
					  int slot, u64 objectid)
{
	u32 nritems = btrfs_header_nritems(leaf);
	struct btrfs_key found_key;
	int scanned = 0;

	slot++;
	while (slot < nritems) {
		btrfs_item_key_to_cpu(leaf, &found_key, slot);

		 
		if (found_key.objectid != objectid)
			return 0;

		 
		if (found_key.type == BTRFS_XATTR_ITEM_KEY)
			return 1;

		 
		if (found_key.type > BTRFS_XATTR_ITEM_KEY)
			return 0;

		slot++;
		scanned++;

		 
		if (scanned >= 8)
			break;
	}
	 
	return 1;
}
