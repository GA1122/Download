static noinline int acls_after_inode_item(struct extent_buffer *leaf,
					  int slot, u64 objectid,
					  int *first_xattr_slot)
{
	u32 nritems = btrfs_header_nritems(leaf);
	struct btrfs_key found_key;
	static u64 xattr_access = 0;
	static u64 xattr_default = 0;
	int scanned = 0;

	if (!xattr_access) {
		xattr_access = btrfs_name_hash(POSIX_ACL_XATTR_ACCESS,
					strlen(POSIX_ACL_XATTR_ACCESS));
		xattr_default = btrfs_name_hash(POSIX_ACL_XATTR_DEFAULT,
					strlen(POSIX_ACL_XATTR_DEFAULT));
	}

	slot++;
	*first_xattr_slot = -1;
	while (slot < nritems) {
		btrfs_item_key_to_cpu(leaf, &found_key, slot);

		 
		if (found_key.objectid != objectid)
			return 0;

		 
		if (found_key.type == BTRFS_XATTR_ITEM_KEY) {
			if (*first_xattr_slot == -1)
				*first_xattr_slot = slot;
			if (found_key.offset == xattr_access ||
			    found_key.offset == xattr_default)
				return 1;
		}

		 
		if (found_key.type > BTRFS_XATTR_ITEM_KEY)
			return 0;

		slot++;
		scanned++;

		 
		if (scanned >= 8)
			break;
	}
	 
	if (*first_xattr_slot == -1)
		*first_xattr_slot = slot;
	return 1;
}
