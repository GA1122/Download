void ext4_truncate(struct inode *inode)
{
	handle_t *handle;
	struct ext4_inode_info *ei = EXT4_I(inode);
	__le32 *i_data = ei->i_data;
	int addr_per_block = EXT4_ADDR_PER_BLOCK(inode->i_sb);
	struct address_space *mapping = inode->i_mapping;
	ext4_lblk_t offsets[4];
	Indirect chain[4];
	Indirect *partial;
	__le32 nr = 0;
	int n;
	ext4_lblk_t last_block;
	unsigned blocksize = inode->i_sb->s_blocksize;

	if (!ext4_can_truncate(inode))
		return;

	EXT4_I(inode)->i_flags &= ~EXT4_EOFBLOCKS_FL;

	if (inode->i_size == 0 && !test_opt(inode->i_sb, NO_AUTO_DA_ALLOC))
		ext4_set_inode_state(inode, EXT4_STATE_DA_ALLOC_CLOSE);

	if (EXT4_I(inode)->i_flags & EXT4_EXTENTS_FL) {
		ext4_ext_truncate(inode);
		return;
	}

	handle = start_transaction(inode);
	if (IS_ERR(handle))
		return;		 

	last_block = (inode->i_size + blocksize-1)
					>> EXT4_BLOCK_SIZE_BITS(inode->i_sb);

	if (inode->i_size & (blocksize - 1))
		if (ext4_block_truncate_page(handle, mapping, inode->i_size))
			goto out_stop;

	n = ext4_block_to_path(inode, last_block, offsets, NULL);
	if (n == 0)
		goto out_stop;	 

	 
	if (ext4_orphan_add(handle, inode))
		goto out_stop;

	 
	down_write(&ei->i_data_sem);

	ext4_discard_preallocations(inode);

	 
	ei->i_disksize = inode->i_size;

	if (n == 1) {		 
		ext4_free_data(handle, inode, NULL, i_data+offsets[0],
			       i_data + EXT4_NDIR_BLOCKS);
		goto do_indirects;
	}

	partial = ext4_find_shared(inode, n, offsets, chain, &nr);
	 
	if (nr) {
		if (partial == chain) {
			 
			ext4_free_branches(handle, inode, NULL,
					   &nr, &nr+1, (chain+n-1) - partial);
			*partial->p = 0;
			 
		} else {
			 
			BUFFER_TRACE(partial->bh, "get_write_access");
			ext4_free_branches(handle, inode, partial->bh,
					partial->p,
					partial->p+1, (chain+n-1) - partial);
		}
	}
	 
	while (partial > chain) {
		ext4_free_branches(handle, inode, partial->bh, partial->p + 1,
				   (__le32*)partial->bh->b_data+addr_per_block,
				   (chain+n-1) - partial);
		BUFFER_TRACE(partial->bh, "call brelse");
		brelse(partial->bh);
		partial--;
	}
do_indirects:
	 
	switch (offsets[0]) {
	default:
		nr = i_data[EXT4_IND_BLOCK];
		if (nr) {
			ext4_free_branches(handle, inode, NULL, &nr, &nr+1, 1);
			i_data[EXT4_IND_BLOCK] = 0;
		}
	case EXT4_IND_BLOCK:
		nr = i_data[EXT4_DIND_BLOCK];
		if (nr) {
			ext4_free_branches(handle, inode, NULL, &nr, &nr+1, 2);
			i_data[EXT4_DIND_BLOCK] = 0;
		}
	case EXT4_DIND_BLOCK:
		nr = i_data[EXT4_TIND_BLOCK];
		if (nr) {
			ext4_free_branches(handle, inode, NULL, &nr, &nr+1, 3);
			i_data[EXT4_TIND_BLOCK] = 0;
		}
	case EXT4_TIND_BLOCK:
		;
	}

	up_write(&ei->i_data_sem);
	inode->i_mtime = inode->i_ctime = ext4_current_time(inode);
	ext4_mark_inode_dirty(handle, inode);

	 
	if (IS_SYNC(inode))
		ext4_handle_sync(handle);
out_stop:
	 
	if (inode->i_nlink)
		ext4_orphan_del(handle, inode);

	ext4_journal_stop(handle);
}
