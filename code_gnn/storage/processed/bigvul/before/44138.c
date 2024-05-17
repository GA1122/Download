int generic_pipe_buf_steal(struct pipe_inode_info *pipe,
			   struct pipe_buffer *buf)
{
	struct page *page = buf->page;

	 
	if (page_count(page) == 1) {
		lock_page(page);
		return 0;
	}

	return 1;
}
