static void ext2_clear_super_error(struct super_block *sb)
{
	struct buffer_head *sbh = EXT2_SB(sb)->s_sbh;

	if (buffer_write_io_error(sbh)) {
		 
		ext2_msg(sb, KERN_ERR,
		       "previous I/O error to superblock detected\n");
		clear_buffer_write_io_error(sbh);
		set_buffer_uptodate(sbh);
	}
}
