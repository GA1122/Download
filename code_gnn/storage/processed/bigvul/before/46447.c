xfs_lookup_buffer_offset(
	struct page		*page,
	loff_t			*offset,
	unsigned int		type)
{
	loff_t			lastoff = page_offset(page);
	bool			found = false;
	struct buffer_head	*bh, *head;

	bh = head = page_buffers(page);
	do {
		 
		if (buffer_unwritten(bh) ||
		    buffer_uptodate(bh)) {
			if (type == DATA_OFF)
				found = true;
		} else {
			if (type == HOLE_OFF)
				found = true;
		}

		if (found) {
			*offset = lastoff;
			break;
		}
		lastoff += bh->b_size;
	} while ((bh = bh->b_this_page) != head);

	return found;
}
