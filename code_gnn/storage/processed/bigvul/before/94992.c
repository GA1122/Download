static int ext4_xattr_block_csum_verify(struct inode *inode,
					sector_t block_nr,
					struct ext4_xattr_header *hdr)
{
	if (ext4_has_metadata_csum(inode->i_sb) &&
	    (hdr->h_checksum != ext4_xattr_block_csum(inode, block_nr, hdr)))
		return 0;
	return 1;
}
