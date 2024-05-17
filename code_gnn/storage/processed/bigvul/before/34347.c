static void btrfs_split_extent_hook(struct inode *inode,
				    struct extent_state *orig, u64 split)
{
	 
	if (!(orig->state & EXTENT_DELALLOC))
		return;

	spin_lock(&BTRFS_I(inode)->lock);
	BTRFS_I(inode)->outstanding_extents++;
	spin_unlock(&BTRFS_I(inode)->lock);
}