static inline int need_force_cow(struct inode *inode, u64 start, u64 end)
{

	if (!(BTRFS_I(inode)->flags & BTRFS_INODE_NODATACOW) &&
	    !(BTRFS_I(inode)->flags & BTRFS_INODE_PREALLOC))
		return 0;

	 
	if (BTRFS_I(inode)->defrag_bytes &&
	    test_range_bit(&BTRFS_I(inode)->io_tree, start, end,
			   EXTENT_DEFRAG, 0, NULL))
		return 1;

	return 0;
}