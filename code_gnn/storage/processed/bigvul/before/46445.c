xfs_find_get_desired_pgoff(
	struct inode		*inode,
	struct xfs_bmbt_irec	*map,
	unsigned int		type,
	loff_t			*offset)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	struct pagevec		pvec;
	pgoff_t			index;
	pgoff_t			end;
	loff_t			endoff;
	loff_t			startoff = *offset;
	loff_t			lastoff = startoff;
	bool			found = false;

	pagevec_init(&pvec, 0);

	index = startoff >> PAGE_CACHE_SHIFT;
	endoff = XFS_FSB_TO_B(mp, map->br_startoff + map->br_blockcount);
	end = endoff >> PAGE_CACHE_SHIFT;
	do {
		int		want;
		unsigned	nr_pages;
		unsigned int	i;

		want = min_t(pgoff_t, end - index, PAGEVEC_SIZE);
		nr_pages = pagevec_lookup(&pvec, inode->i_mapping, index,
					  want);
		 
		if (nr_pages == 0) {
			 
			if (type == DATA_OFF)
				break;

			ASSERT(type == HOLE_OFF);
			if (lastoff == startoff || lastoff < endoff) {
				found = true;
				*offset = lastoff;
			}
			break;
		}

		 
		if (type == HOLE_OFF && lastoff == startoff &&
		    lastoff < page_offset(pvec.pages[0])) {
			found = true;
			break;
		}

		for (i = 0; i < nr_pages; i++) {
			struct page	*page = pvec.pages[i];
			loff_t		b_offset;

			 
			if (page->index > end) {
				if (type == HOLE_OFF && lastoff < endoff) {
					*offset = lastoff;
					found = true;
				}
				goto out;
			}

			lock_page(page);
			 
			if (unlikely(page->mapping != inode->i_mapping)) {
				unlock_page(page);
				continue;
			}

			if (!page_has_buffers(page)) {
				unlock_page(page);
				continue;
			}

			found = xfs_lookup_buffer_offset(page, &b_offset, type);
			if (found) {
				 
				*offset = max_t(loff_t, startoff, b_offset);
				unlock_page(page);
				goto out;
			}

			 
			lastoff = page_offset(page) + PAGE_SIZE;
			unlock_page(page);
		}

		 
		if (nr_pages < want) {
			if (type == HOLE_OFF) {
				*offset = lastoff;
				found = true;
			}
			break;
		}

		index = pvec.pages[i - 1]->index + 1;
		pagevec_release(&pvec);
	} while (index <= end);

out:
	pagevec_release(&pvec);
	return found;
}
