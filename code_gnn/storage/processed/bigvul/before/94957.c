static int ext2_freeze(struct super_block *sb)
{
	struct ext2_sb_info *sbi = EXT2_SB(sb);

	 
	if (atomic_long_read(&sb->s_remove_count)) {
		ext2_sync_fs(sb, 1);
		return 0;
	}
	 
	spin_lock(&sbi->s_lock);
	sbi->s_es->s_state = cpu_to_le16(sbi->s_mount_state);
	spin_unlock(&sbi->s_lock);
	ext2_sync_super(sb, sbi->s_es, 1);

	return 0;
}
