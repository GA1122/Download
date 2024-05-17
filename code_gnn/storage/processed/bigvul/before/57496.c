static void ext4_da_release_space(struct inode *inode, int to_free)
{
	struct ext4_sb_info *sbi = EXT4_SB(inode->i_sb);
	struct ext4_inode_info *ei = EXT4_I(inode);

	if (!to_free)
		return;		 

	spin_lock(&EXT4_I(inode)->i_block_reservation_lock);

	if (unlikely(to_free > ei->i_reserved_data_blocks)) {
		 
		ext4_msg(inode->i_sb, KERN_NOTICE, "ext4_da_release_space: "
			 "ino %lu, to_free %d with only %d reserved "
			 "data blocks\n", inode->i_ino, to_free,
			 ei->i_reserved_data_blocks);
		WARN_ON(1);
		to_free = ei->i_reserved_data_blocks;
	}
	ei->i_reserved_data_blocks -= to_free;

	if (ei->i_reserved_data_blocks == 0) {
		 
		to_free += ei->i_reserved_meta_blocks;
		ei->i_reserved_meta_blocks = 0;
		ei->i_da_metadata_calc_len = 0;
	}

	 
	percpu_counter_sub(&sbi->s_dirtyblocks_counter, to_free);

	spin_unlock(&EXT4_I(inode)->i_block_reservation_lock);

	vfs_dq_release_reservation_block(inode, to_free);
}
