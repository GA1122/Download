static __le32 ext4_xattr_block_csum(struct inode *inode,
				    sector_t block_nr,
				    struct ext4_xattr_header *hdr)
{
	struct ext4_sb_info *sbi = EXT4_SB(inode->i_sb);
	__u32 csum;
	__le32 save_csum;
	__le64 dsk_block_nr = cpu_to_le64(block_nr);

	save_csum = hdr->h_checksum;
	hdr->h_checksum = 0;
	csum = ext4_chksum(sbi, sbi->s_csum_seed, (__u8 *)&dsk_block_nr,
			   sizeof(dsk_block_nr));
	csum = ext4_chksum(sbi, csum, (__u8 *)hdr,
			   EXT4_BLOCK_SIZE(inode->i_sb));

	hdr->h_checksum = save_csum;
	return cpu_to_le32(csum);
}
