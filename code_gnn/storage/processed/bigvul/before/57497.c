static int ext4_da_reserve_space(struct inode *inode, sector_t lblock)
{
	int retries = 0;
	struct ext4_sb_info *sbi = EXT4_SB(inode->i_sb);
	struct ext4_inode_info *ei = EXT4_I(inode);
	unsigned long md_needed, md_reserved;

	 
repeat:
	spin_lock(&ei->i_block_reservation_lock);
	md_reserved = ei->i_reserved_meta_blocks;
	md_needed = ext4_calc_metadata_amount(inode, lblock);
	trace_ext4_da_reserve_space(inode, md_needed);
	spin_unlock(&ei->i_block_reservation_lock);

	 
	if (vfs_dq_reserve_block(inode, md_needed + 1))
		return -EDQUOT;

	if (ext4_claim_free_blocks(sbi, md_needed + 1)) {
		vfs_dq_release_reservation_block(inode, md_needed + 1);
		if (ext4_should_retry_alloc(inode->i_sb, &retries)) {
			yield();
			goto repeat;
		}
		return -ENOSPC;
	}
	spin_lock(&ei->i_block_reservation_lock);
	ei->i_reserved_data_blocks++;
	ei->i_reserved_meta_blocks += md_needed;
	spin_unlock(&ei->i_block_reservation_lock);

	return 0;        
}
