static ssize_t session_write_kbytes_show(struct ext4_attr *a,
					 struct ext4_sb_info *sbi, char *buf)
{
	struct super_block *sb = sbi->s_buddy_cache->i_sb;

	if (!sb->s_bdev->bd_part)
		return snprintf(buf, PAGE_SIZE, "0\n");
	return snprintf(buf, PAGE_SIZE, "%lu\n",
			(part_stat_read(sb->s_bdev->bd_part, sectors[1]) -
			 sbi->s_sectors_written_start) >> 1);
}