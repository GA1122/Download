static int fuse_writepages_fill(struct page *page,
		struct writeback_control *wbc, void *_data)
{
	struct fuse_fill_wb_data *data = _data;
	struct fuse_req *req = data->req;
	struct inode *inode = data->inode;
	struct fuse_conn *fc = get_fuse_conn(inode);
	struct page *tmp_page;
	bool is_writeback;
	int err;

	if (!data->ff) {
		err = -EIO;
		data->ff = fuse_write_file_get(fc, get_fuse_inode(inode));
		if (!data->ff)
			goto out_unlock;
	}

	 
	is_writeback = fuse_page_is_writeback(inode, page->index);

	if (req && req->num_pages &&
	    (is_writeback || req->num_pages == FUSE_MAX_PAGES_PER_REQ ||
	     (req->num_pages + 1) * PAGE_CACHE_SIZE > fc->max_write ||
	     data->orig_pages[req->num_pages - 1]->index + 1 != page->index)) {
		fuse_writepages_send(data);
		data->req = NULL;
	}
	err = -ENOMEM;
	tmp_page = alloc_page(GFP_NOFS | __GFP_HIGHMEM);
	if (!tmp_page)
		goto out_unlock;

	 
	if (data->req == NULL) {
		struct fuse_inode *fi = get_fuse_inode(inode);

		err = -ENOMEM;
		req = fuse_request_alloc_nofs(FUSE_MAX_PAGES_PER_REQ);
		if (!req) {
			__free_page(tmp_page);
			goto out_unlock;
		}

		fuse_write_fill(req, data->ff, page_offset(page), 0);
		req->misc.write.in.write_flags |= FUSE_WRITE_CACHE;
		req->misc.write.next = NULL;
		req->in.argpages = 1;
		__set_bit(FR_BACKGROUND, &req->flags);
		req->num_pages = 0;
		req->end = fuse_writepage_end;
		req->inode = inode;

		spin_lock(&fc->lock);
		list_add(&req->writepages_entry, &fi->writepages);
		spin_unlock(&fc->lock);

		data->req = req;
	}
	set_page_writeback(page);

	copy_highpage(tmp_page, page);
	req->pages[req->num_pages] = tmp_page;
	req->page_descs[req->num_pages].offset = 0;
	req->page_descs[req->num_pages].length = PAGE_SIZE;

	inc_wb_stat(&inode_to_bdi(inode)->wb, WB_WRITEBACK);
	inc_zone_page_state(tmp_page, NR_WRITEBACK_TEMP);

	err = 0;
	if (is_writeback && fuse_writepage_in_flight(req, page)) {
		end_page_writeback(page);
		data->req = NULL;
		goto out_unlock;
	}
	data->orig_pages[req->num_pages] = page;

	 
	spin_lock(&fc->lock);
	req->num_pages++;
	spin_unlock(&fc->lock);

out_unlock:
	unlock_page(page);

	return err;
}