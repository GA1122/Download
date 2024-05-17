static int ext4_da_reserve_space(struct inode *inode)
{
	struct ext4_sb_info *sbi = EXT4_SB(inode->i_sb);
	struct ext4_inode_info *ei = EXT4_I(inode);
	int ret;

	 
	ret = dquot_reserve_block(inode, EXT4_C2B(sbi, 1));
	if (ret)
		return ret;

	spin_lock(&ei->i_block_reservation_lock);
	if (ext4_claim_free_clusters(sbi, 1, 0)) {
		spin_unlock(&ei->i_block_reservation_lock);
		dquot_release_reservation_block(inode, EXT4_C2B(sbi, 1));
		return -ENOSPC;
	}
	ei->i_reserved_data_blocks++;
	trace_ext4_da_reserve_space(inode);
	spin_unlock(&ei->i_block_reservation_lock);

	return 0;        
}