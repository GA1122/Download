static void ext4_free_branches(handle_t *handle, struct inode *inode,
			       struct buffer_head *parent_bh,
			       __le32 *first, __le32 *last, int depth)
{
	ext4_fsblk_t nr;
	__le32 *p;

	if (ext4_handle_is_aborted(handle))
		return;

	if (depth--) {
		struct buffer_head *bh;
		int addr_per_block = EXT4_ADDR_PER_BLOCK(inode->i_sb);
		p = last;
		while (--p >= first) {
			nr = le32_to_cpu(*p);
			if (!nr)
				continue;		 

			if (!ext4_data_block_valid(EXT4_SB(inode->i_sb),
						   nr, 1)) {
				ext4_error(inode->i_sb,
					   "indirect mapped block in inode "
					   "#%lu invalid (level %d, blk #%lu)",
					   inode->i_ino, depth,
					   (unsigned long) nr);
				break;
			}

			 
			bh = sb_bread(inode->i_sb, nr);

			 
			if (!bh) {
				ext4_error(inode->i_sb,
					   "Read failure, inode=%lu, block=%llu",
					   inode->i_ino, nr);
				continue;
			}

			 
			BUFFER_TRACE(bh, "free child branches");
			ext4_free_branches(handle, inode, bh,
					(__le32 *) bh->b_data,
					(__le32 *) bh->b_data + addr_per_block,
					depth);

			 
			ext4_forget(handle, 1, inode, bh, bh->b_blocknr);

			 
			if (ext4_handle_is_aborted(handle))
				return;
			if (try_to_extend_transaction(handle, inode)) {
				ext4_mark_inode_dirty(handle, inode);
				ext4_truncate_restart_trans(handle, inode,
					    blocks_for_truncate(inode));
			}

			ext4_free_blocks(handle, inode, 0, nr, 1,
					 EXT4_FREE_BLOCKS_METADATA);

			if (parent_bh) {
				 
				BUFFER_TRACE(parent_bh, "get_write_access");
				if (!ext4_journal_get_write_access(handle,
								   parent_bh)){
					*p = 0;
					BUFFER_TRACE(parent_bh,
					"call ext4_handle_dirty_metadata");
					ext4_handle_dirty_metadata(handle,
								   inode,
								   parent_bh);
				}
			}
		}
	} else {
		 
		BUFFER_TRACE(parent_bh, "free data blocks");
		ext4_free_data(handle, inode, parent_bh, first, last);
	}
}
