static int ext3_commit_super(struct super_block *sb,
			       struct ext3_super_block *es,
			       int sync)
{
	struct buffer_head *sbh = EXT3_SB(sb)->s_sbh;
	int error = 0;

	if (!sbh)
		return error;

	if (buffer_write_io_error(sbh)) {
		 
		ext3_msg(sb, KERN_ERR, "previous I/O error to "
		       "superblock detected");
		clear_buffer_write_io_error(sbh);
		set_buffer_uptodate(sbh);
	}
	 
	if (!(sb->s_flags & MS_RDONLY))
		es->s_wtime = cpu_to_le32(get_seconds());
	es->s_free_blocks_count = cpu_to_le32(ext3_count_free_blocks(sb));
	es->s_free_inodes_count = cpu_to_le32(ext3_count_free_inodes(sb));
	BUFFER_TRACE(sbh, "marking dirty");
	mark_buffer_dirty(sbh);
	if (sync) {
		error = sync_dirty_buffer(sbh);
		if (buffer_write_io_error(sbh)) {
			ext3_msg(sb, KERN_ERR, "I/O error while writing "
			       "superblock");
			clear_buffer_write_io_error(sbh);
			set_buffer_uptodate(sbh);
		}
	}
	return error;
}
