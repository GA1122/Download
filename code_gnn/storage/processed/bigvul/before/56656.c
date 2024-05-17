static int ext4_commit_super(struct super_block *sb, int sync)
{
	struct ext4_super_block *es = EXT4_SB(sb)->s_es;
	struct buffer_head *sbh = EXT4_SB(sb)->s_sbh;
	int error = 0;

	if (!sbh || block_device_ejected(sb))
		return error;
	if (buffer_write_io_error(sbh)) {
		 
		ext4_msg(sb, KERN_ERR, "previous I/O error to "
		       "superblock detected");
		clear_buffer_write_io_error(sbh);
		set_buffer_uptodate(sbh);
	}
	 
	if (!(sb->s_flags & MS_RDONLY))
		es->s_wtime = cpu_to_le32(get_seconds());
	if (sb->s_bdev->bd_part)
		es->s_kbytes_written =
			cpu_to_le64(EXT4_SB(sb)->s_kbytes_written +
			    ((part_stat_read(sb->s_bdev->bd_part, sectors[1]) -
			      EXT4_SB(sb)->s_sectors_written_start) >> 1));
	else
		es->s_kbytes_written =
			cpu_to_le64(EXT4_SB(sb)->s_kbytes_written);
	if (percpu_counter_initialized(&EXT4_SB(sb)->s_freeclusters_counter))
		ext4_free_blocks_count_set(es,
			EXT4_C2B(EXT4_SB(sb), percpu_counter_sum_positive(
				&EXT4_SB(sb)->s_freeclusters_counter)));
	if (percpu_counter_initialized(&EXT4_SB(sb)->s_freeinodes_counter))
		es->s_free_inodes_count =
			cpu_to_le32(percpu_counter_sum_positive(
				&EXT4_SB(sb)->s_freeinodes_counter));
	BUFFER_TRACE(sbh, "marking dirty");
	ext4_superblock_csum_set(sb);
	mark_buffer_dirty(sbh);
	if (sync) {
		error = __sync_dirty_buffer(sbh,
			test_opt(sb, BARRIER) ? WRITE_FUA : WRITE_SYNC);
		if (error)
			return error;

		error = buffer_write_io_error(sbh);
		if (error) {
			ext4_msg(sb, KERN_ERR, "I/O error while writing "
			       "superblock");
			clear_buffer_write_io_error(sbh);
			set_buffer_uptodate(sbh);
		}
	}
	return error;
}