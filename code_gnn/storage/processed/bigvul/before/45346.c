read_block_for_search(struct btrfs_trans_handle *trans,
		       struct btrfs_root *root, struct btrfs_path *p,
		       struct extent_buffer **eb_ret, int level, int slot,
		       struct btrfs_key *key, u64 time_seq)
{
	u64 blocknr;
	u64 gen;
	struct extent_buffer *b = *eb_ret;
	struct extent_buffer *tmp;
	int ret;

	blocknr = btrfs_node_blockptr(b, slot);
	gen = btrfs_node_ptr_generation(b, slot);

	tmp = btrfs_find_tree_block(root, blocknr);
	if (tmp) {
		 
		if (btrfs_buffer_uptodate(tmp, gen, 1) > 0) {
			*eb_ret = tmp;
			return 0;
		}

		 
		btrfs_set_path_blocking(p);

		 
		ret = btrfs_read_buffer(tmp, gen);
		if (!ret) {
			*eb_ret = tmp;
			return 0;
		}
		free_extent_buffer(tmp);
		btrfs_release_path(p);
		return -EIO;
	}

	 
	btrfs_unlock_up_safe(p, level + 1);
	btrfs_set_path_blocking(p);

	free_extent_buffer(tmp);
	if (p->reada)
		reada_for_search(root, p, level, slot, key->objectid);

	btrfs_release_path(p);

	ret = -EAGAIN;
	tmp = read_tree_block(root, blocknr, 0);
	if (tmp) {
		 
		if (!btrfs_buffer_uptodate(tmp, 0, 0))
			ret = -EIO;
		free_extent_buffer(tmp);
	}
	return ret;
}
