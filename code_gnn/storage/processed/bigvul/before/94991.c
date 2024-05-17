static void ext4_xattr_block_csum_set(struct inode *inode,
				      sector_t block_nr,
				      struct ext4_xattr_header *hdr)
{
	if (!ext4_has_metadata_csum(inode->i_sb))
		return;

	hdr->h_checksum = ext4_xattr_block_csum(inode, block_nr, hdr);
}
