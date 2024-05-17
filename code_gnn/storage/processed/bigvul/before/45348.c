static noinline void reada_for_balance(struct btrfs_root *root,
				       struct btrfs_path *path, int level)
{
	int slot;
	int nritems;
	struct extent_buffer *parent;
	struct extent_buffer *eb;
	u64 gen;
	u64 block1 = 0;
	u64 block2 = 0;
	int blocksize;

	parent = path->nodes[level + 1];
	if (!parent)
		return;

	nritems = btrfs_header_nritems(parent);
	slot = path->slots[level + 1];
	blocksize = root->nodesize;

	if (slot > 0) {
		block1 = btrfs_node_blockptr(parent, slot - 1);
		gen = btrfs_node_ptr_generation(parent, slot - 1);
		eb = btrfs_find_tree_block(root, block1);
		 
		if (eb && btrfs_buffer_uptodate(eb, gen, 1) != 0)
			block1 = 0;
		free_extent_buffer(eb);
	}
	if (slot + 1 < nritems) {
		block2 = btrfs_node_blockptr(parent, slot + 1);
		gen = btrfs_node_ptr_generation(parent, slot + 1);
		eb = btrfs_find_tree_block(root, block2);
		if (eb && btrfs_buffer_uptodate(eb, gen, 1) != 0)
			block2 = 0;
		free_extent_buffer(eb);
	}

	if (block1)
		readahead_tree_block(root, block1, blocksize);
	if (block2)
		readahead_tree_block(root, block2, blocksize);
}