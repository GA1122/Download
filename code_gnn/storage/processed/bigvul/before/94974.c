void ext2_write_super(struct super_block *sb)
{
	if (!(sb->s_flags & MS_RDONLY))
		ext2_sync_fs(sb, 1);
}
