int verify_dir_item(struct btrfs_root *root,
		    struct extent_buffer *leaf,
		    struct btrfs_dir_item *dir_item)
{
	u16 namelen = BTRFS_NAME_LEN;
	u8 type = btrfs_dir_type(leaf, dir_item);

	if (type >= BTRFS_FT_MAX) {
		printk(KERN_CRIT "btrfs: invalid dir item type: %d\n",
		       (int)type);
		return 1;
	}

	if (type == BTRFS_FT_XATTR)
		namelen = XATTR_NAME_MAX;

	if (btrfs_dir_name_len(leaf, dir_item) > namelen) {
		printk(KERN_CRIT "btrfs: invalid dir item name len: %u\n",
		       (unsigned)btrfs_dir_data_len(leaf, dir_item));
		return 1;
	}

	 
	if (btrfs_dir_data_len(leaf, dir_item) > BTRFS_MAX_XATTR_SIZE(root)) {
		printk(KERN_CRIT "btrfs: invalid dir item data len: %u\n",
		       (unsigned)btrfs_dir_data_len(leaf, dir_item));
		return 1;
	}

	return 0;
}