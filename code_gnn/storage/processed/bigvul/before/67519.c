static int ext4_dio_get_block_unwritten_async(struct inode *inode,
		sector_t iblock, struct buffer_head *bh_result,	int create)
{
	int ret;

	 
	WARN_ON_ONCE(ext4_journal_current_handle());

	ret = ext4_get_block_trans(inode, iblock, bh_result,
				   EXT4_GET_BLOCKS_IO_CREATE_EXT);

	 
	if (!ret && buffer_unwritten(bh_result)) {
		if (!bh_result->b_private) {
			ext4_io_end_t *io_end;

			io_end = ext4_init_io_end(inode, GFP_KERNEL);
			if (!io_end)
				return -ENOMEM;
			bh_result->b_private = io_end;
			ext4_set_io_unwritten_flag(inode, io_end);
		}
		set_buffer_defer_completion(bh_result);
	}

	return ret;
}
