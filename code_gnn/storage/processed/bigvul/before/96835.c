static int fuse_try_move_page(struct fuse_copy_state *cs, struct page **pagep)
{
	int err;
	struct page *oldpage = *pagep;
	struct page *newpage;
	struct pipe_buffer *buf = cs->pipebufs;

	err = unlock_request(cs->req);
	if (err)
		return err;

	fuse_copy_finish(cs);

	err = pipe_buf_confirm(cs->pipe, buf);
	if (err)
		return err;

	BUG_ON(!cs->nr_segs);
	cs->currbuf = buf;
	cs->len = buf->len;
	cs->pipebufs++;
	cs->nr_segs--;

	if (cs->len != PAGE_SIZE)
		goto out_fallback;

	if (pipe_buf_steal(cs->pipe, buf) != 0)
		goto out_fallback;

	newpage = buf->page;

	if (!PageUptodate(newpage))
		SetPageUptodate(newpage);

	ClearPageMappedToDisk(newpage);

	if (fuse_check_page(newpage) != 0)
		goto out_fallback_unlock;

	 
	if (WARN_ON(page_mapped(oldpage)))
		goto out_fallback_unlock;
	if (WARN_ON(page_has_private(oldpage)))
		goto out_fallback_unlock;
	if (WARN_ON(PageDirty(oldpage) || PageWriteback(oldpage)))
		goto out_fallback_unlock;
	if (WARN_ON(PageMlocked(oldpage)))
		goto out_fallback_unlock;

	err = replace_page_cache_page(oldpage, newpage, GFP_KERNEL);
	if (err) {
		unlock_page(newpage);
		return err;
	}

	get_page(newpage);

	if (!(buf->flags & PIPE_BUF_FLAG_LRU))
		lru_cache_add_file(newpage);

	err = 0;
	spin_lock(&cs->req->waitq.lock);
	if (test_bit(FR_ABORTED, &cs->req->flags))
		err = -ENOENT;
	else
		*pagep = newpage;
	spin_unlock(&cs->req->waitq.lock);

	if (err) {
		unlock_page(newpage);
		put_page(newpage);
		return err;
	}

	unlock_page(oldpage);
	put_page(oldpage);
	cs->len = 0;

	return 0;

out_fallback_unlock:
	unlock_page(newpage);
out_fallback:
	cs->pg = buf->page;
	cs->offset = buf->offset;

	err = lock_request(cs->req);
	if (err)
		return err;

	return 1;
}