static int ext4_indirect_trans_blocks(struct inode *inode, int nrblocks,
				      int chunk)
{
	int indirects;

	 
	if (chunk) {
		 
		indirects = nrblocks / EXT4_ADDR_PER_BLOCK(inode->i_sb);
		return indirects + 3;
	}
	 
	indirects = nrblocks * 2 + 1;
	return indirects;
}
