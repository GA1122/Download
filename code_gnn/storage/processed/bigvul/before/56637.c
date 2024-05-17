static void __save_error_info(struct super_block *sb, const char *func,
			    unsigned int line)
{
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;

	EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
	if (bdev_read_only(sb->s_bdev))
		return;
	es->s_state |= cpu_to_le16(EXT4_ERROR_FS);
	es->s_last_error_time = cpu_to_le32(get_seconds());
	strncpy(es->s_last_error_func, func, sizeof(es->s_last_error_func));
	es->s_last_error_line = cpu_to_le32(line);
	if (!es->s_first_error_time) {
		es->s_first_error_time = es->s_last_error_time;
		strncpy(es->s_first_error_func, func,
			sizeof(es->s_first_error_func));
		es->s_first_error_line = cpu_to_le32(line);
		es->s_first_error_ino = es->s_last_error_ino;
		es->s_first_error_block = es->s_last_error_block;
	}
	 
	if (!es->s_error_count)
		mod_timer(&EXT4_SB(sb)->s_err_report, jiffies + 24*60*60*HZ);
	le32_add_cpu(&es->s_error_count, 1);
}