static void btrfs_clear_bit_hook(struct inode *inode,
				 struct extent_state *state, int *bits)
{
	 
	if ((state->state & EXTENT_DELALLOC) && (*bits & EXTENT_DELALLOC)) {
		struct btrfs_root *root = BTRFS_I(inode)->root;
		u64 len = state->end + 1 - state->start;
		bool do_list = !btrfs_is_free_space_inode(inode);

		if (*bits & EXTENT_FIRST_DELALLOC) {
			*bits &= ~EXTENT_FIRST_DELALLOC;
		} else if (!(*bits & EXTENT_DO_ACCOUNTING)) {
			spin_lock(&BTRFS_I(inode)->lock);
			BTRFS_I(inode)->outstanding_extents--;
			spin_unlock(&BTRFS_I(inode)->lock);
		}

		if (*bits & EXTENT_DO_ACCOUNTING)
			btrfs_delalloc_release_metadata(inode, len);

		if (root->root_key.objectid != BTRFS_DATA_RELOC_TREE_OBJECTID
		    && do_list)
			btrfs_free_reserved_data_space(inode, len);

		spin_lock(&root->fs_info->delalloc_lock);
		root->fs_info->delalloc_bytes -= len;
		BTRFS_I(inode)->delalloc_bytes -= len;

		if (do_list && BTRFS_I(inode)->delalloc_bytes == 0 &&
		    !list_empty(&BTRFS_I(inode)->delalloc_inodes)) {
			list_del_init(&BTRFS_I(inode)->delalloc_inodes);
		}
		spin_unlock(&root->fs_info->delalloc_lock);
	}
}