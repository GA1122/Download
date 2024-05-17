void ext2_error(struct super_block *sb, const char *function,
		const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	struct ext2_sb_info *sbi = EXT2_SB(sb);
	struct ext2_super_block *es = sbi->s_es;

	if (!(sb->s_flags & MS_RDONLY)) {
		spin_lock(&sbi->s_lock);
		sbi->s_mount_state |= EXT2_ERROR_FS;
		es->s_state |= cpu_to_le16(EXT2_ERROR_FS);
		spin_unlock(&sbi->s_lock);
		ext2_sync_super(sb, es, 1);
	}

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	printk(KERN_CRIT "EXT2-fs (%s): error: %s: %pV\n",
	       sb->s_id, function, &vaf);

	va_end(args);

	if (test_opt(sb, ERRORS_PANIC))
		panic("EXT2-fs: panic from previous error\n");
	if (test_opt(sb, ERRORS_RO)) {
		ext2_msg(sb, KERN_CRIT,
			     "error: remounting filesystem read-only");
		sb->s_flags |= MS_RDONLY;
	}
}
