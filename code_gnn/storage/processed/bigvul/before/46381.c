static int page_cache_pipe_buf_confirm(struct pipe_inode_info *pipe,
				       struct pipe_buffer *buf)
{
	struct page *page = buf->page;
	int err;

	if (!PageUptodate(page)) {
		lock_page(page);

		 
		if (!page->mapping) {
			err = -ENODATA;
			goto error;
		}

		 
		if (!PageUptodate(page)) {
			err = -EIO;
			goto error;
		}

		 
		unlock_page(page);
	}

	return 0;
error:
	unlock_page(page);
	return err;
}
