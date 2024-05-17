int btrfs_orphan_del(struct btrfs_trans_handle *trans, struct inode *inode)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	int delete_item = 0;
	int release_rsv = 0;
	int ret = 0;

	spin_lock(&root->orphan_lock);
	if (test_and_clear_bit(BTRFS_INODE_HAS_ORPHAN_ITEM,
			       &BTRFS_I(inode)->runtime_flags))
		delete_item = 1;

	if (test_and_clear_bit(BTRFS_INODE_ORPHAN_META_RESERVED,
			       &BTRFS_I(inode)->runtime_flags))
		release_rsv = 1;
	spin_unlock(&root->orphan_lock);

	if (trans && delete_item) {
		ret = btrfs_del_orphan_item(trans, root, btrfs_ino(inode));
		BUG_ON(ret);  
	}

	if (release_rsv) {
		btrfs_orphan_release_metadata(inode);
		atomic_dec(&root->orphan_inodes);
	}

	return 0;
}