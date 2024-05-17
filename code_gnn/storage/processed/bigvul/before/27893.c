static void fuse_readpages_end(struct fuse_conn *fc, struct fuse_req *req)
{
	int i;
	size_t count = req->misc.read.in.size;
	size_t num_read = req->out.args[0].size;
	struct address_space *mapping = NULL;

	for (i = 0; mapping == NULL && i < req->num_pages; i++)
		mapping = req->pages[i]->mapping;

	if (mapping) {
		struct inode *inode = mapping->host;

		 
		if (!req->out.h.error && num_read < count) {
			loff_t pos;

			pos = page_offset(req->pages[0]) + num_read;
			fuse_read_update_size(inode, pos,
					      req->misc.read.attr_ver);
		}
		fuse_invalidate_attr(inode);  
	}

	for (i = 0; i < req->num_pages; i++) {
		struct page *page = req->pages[i];
		if (!req->out.h.error)
			SetPageUptodate(page);
		else
			SetPageError(page);
		unlock_page(page);
		page_cache_release(page);
	}
	if (req->ff)
		fuse_file_put(req->ff);
}