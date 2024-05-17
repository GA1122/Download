static int ext4_ext_fiemap_cb(struct inode *inode, ext4_lblk_t next,
		       struct ext4_ext_cache *newex, struct ext4_extent *ex,
		       void *data)
{
	__u64	logical;
	__u64	physical;
	__u64	length;
	__u32	flags = 0;
	int		ret = 0;
	struct fiemap_extent_info *fieinfo = data;
	unsigned char blksize_bits;

	blksize_bits = inode->i_sb->s_blocksize_bits;
	logical = (__u64)newex->ec_block << blksize_bits;

	if (newex->ec_start == 0) {
		 
		ext4_lblk_t	end = 0;
		pgoff_t		last_offset;
		pgoff_t		offset;
		pgoff_t		index;
		pgoff_t		start_index = 0;
		struct page	**pages = NULL;
		struct buffer_head *bh = NULL;
		struct buffer_head *head = NULL;
		unsigned int nr_pages = PAGE_SIZE / sizeof(struct page *);

		pages = kmalloc(PAGE_SIZE, GFP_KERNEL);
		if (pages == NULL)
			return -ENOMEM;

		offset = logical >> PAGE_SHIFT;
repeat:
		last_offset = offset;
		head = NULL;
		ret = find_get_pages_tag(inode->i_mapping, &offset,
					PAGECACHE_TAG_DIRTY, nr_pages, pages);

		if (!(flags & FIEMAP_EXTENT_DELALLOC)) {
			 
			if (ret == 0) {
out:
				for (index = 0; index < ret; index++)
					page_cache_release(pages[index]);
				 
				kfree(pages);
				return EXT_CONTINUE;
			}
			index = 0;

next_page:
			 
			end = ((__u64)pages[index]->index << PAGE_SHIFT) >>
				  blksize_bits;
			if (!page_has_buffers(pages[index]))
				goto out;
			head = page_buffers(pages[index]);
			if (!head)
				goto out;

			index++;
			bh = head;
			do {
				if (end >= newex->ec_block +
					newex->ec_len)
					 
					goto out;

				if (buffer_mapped(bh) &&
				    end >= newex->ec_block) {
					start_index = index - 1;
					 
					goto found_mapped_buffer;
				}

				bh = bh->b_this_page;
				end++;
			} while (bh != head);

			 
			if (index >= ret) {
				 
				newex->ec_len = end - newex->ec_block;
				goto out;
			}
			goto next_page;
		} else {
			 
			if (ret > 0 && pages[0]->index == last_offset)
				head = page_buffers(pages[0]);
			bh = head;
			index = 1;
			start_index = 0;
		}

found_mapped_buffer:
		if (bh != NULL && buffer_delay(bh)) {
			 
			if (!(flags & FIEMAP_EXTENT_DELALLOC)) {
				 
				flags |= FIEMAP_EXTENT_DELALLOC;
				newex->ec_block = end;
				logical = (__u64)end << blksize_bits;
			}
			 
			do {
				if (!buffer_delay(bh))
					goto found_delayed_extent;
				bh = bh->b_this_page;
				end++;
			} while (bh != head);

			for (; index < ret; index++) {
				if (!page_has_buffers(pages[index])) {
					bh = NULL;
					break;
				}
				head = page_buffers(pages[index]);
				if (!head) {
					bh = NULL;
					break;
				}

				if (pages[index]->index !=
				    pages[start_index]->index + index
				    - start_index) {
					 
					bh = NULL;
					break;
				}
				bh = head;
				do {
					if (!buffer_delay(bh))
						 
						goto found_delayed_extent;
					bh = bh->b_this_page;
					end++;
				} while (bh != head);
			}
		} else if (!(flags & FIEMAP_EXTENT_DELALLOC))
			 
			goto out;

found_delayed_extent:
		newex->ec_len = min(end - newex->ec_block,
						(ext4_lblk_t)EXT_INIT_MAX_LEN);
		if (ret == nr_pages && bh != NULL &&
			newex->ec_len < EXT_INIT_MAX_LEN &&
			buffer_delay(bh)) {
			 
			for (index = 0; index < ret; index++)
				page_cache_release(pages[index]);
			goto repeat;
		}

		for (index = 0; index < ret; index++)
			page_cache_release(pages[index]);
		kfree(pages);
	}

	physical = (__u64)newex->ec_start << blksize_bits;
	length =   (__u64)newex->ec_len << blksize_bits;

	if (ex && ext4_ext_is_uninitialized(ex))
		flags |= FIEMAP_EXTENT_UNWRITTEN;

	if (next == EXT_MAX_BLOCKS)
		flags |= FIEMAP_EXTENT_LAST;

	ret = fiemap_fill_next_extent(fieinfo, logical, physical,
					length, flags);
	if (ret < 0)
		return ret;
	if (ret == 1)
		return EXT_BREAK;
	return EXT_CONTINUE;
}
