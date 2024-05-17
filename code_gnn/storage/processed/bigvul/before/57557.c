__releases(bitlock)
__acquires(bitlock)
{
	va_list args;
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;

	va_start(args, fmt);
	printk(KERN_CRIT "EXT4-fs error (device %s): %s: ", sb->s_id, function);
	vprintk(fmt, args);
	printk("\n");
	va_end(args);

	if (test_opt(sb, ERRORS_CONT)) {
		EXT4_SB(sb)->s_mount_state |= EXT4_ERROR_FS;
		es->s_state |= cpu_to_le16(EXT4_ERROR_FS);
		ext4_commit_super(sb, 0);
		return;
	}
	ext4_unlock_group(sb, grp);
	ext4_handle_error(sb);
	 
	ext4_lock_group(sb, grp);
	return;
}
