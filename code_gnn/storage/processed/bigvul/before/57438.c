static int ext4_ext_fiemap_cb(struct inode *inode, struct ext4_ext_path *path,
		       struct ext4_ext_cache *newex, struct ext4_extent *ex,
		       void *data)
{
	struct fiemap_extent_info *fieinfo = data;
	unsigned char blksize_bits = inode->i_sb->s_blocksize_bits;
	__u64	logical;
	__u64	physical;
	__u64	length;
	__u32	flags = 0;
	int	error;

	logical =  (__u64)newex->ec_block << blksize_bits;

	if (newex->ec_type == EXT4_EXT_CACHE_GAP) {
		pgoff_t offset;
		struct page *page;
		struct buffer_head *bh = NULL;

		offset = logical >> PAGE_SHIFT;
		page = find_get_page(inode->i_mapping, offset);
		if (!page || !page_has_buffers(page))
			return EXT_CONTINUE;

		bh = page_buffers(page);

		if (!bh)
			return EXT_CONTINUE;

		if (buffer_delay(bh)) {
			flags |= FIEMAP_EXTENT_DELALLOC;
			page_cache_release(page);
		} else {
			page_cache_release(page);
			return EXT_CONTINUE;
		}
	}

	physical = (__u64)newex->ec_start << blksize_bits;
	length =   (__u64)newex->ec_len << blksize_bits;

	if (ex && ext4_ext_is_uninitialized(ex))
		flags |= FIEMAP_EXTENT_UNWRITTEN;

	 
	if (ext4_ext_next_allocated_block(path) == EXT_MAX_BLOCK ||
	    newex->ec_block + newex->ec_len - 1 == EXT_MAX_BLOCK) {
		loff_t size = i_size_read(inode);
		loff_t bs = EXT4_BLOCK_SIZE(inode->i_sb);

		flags |= FIEMAP_EXTENT_LAST;
		if ((flags & FIEMAP_EXTENT_DELALLOC) &&
		    logical+length > size)
			length = (size - logical + bs - 1) & ~(bs-1);
	}

	error = fiemap_fill_next_extent(fieinfo, logical, physical,
					length, flags);
	if (error < 0)
		return error;
	if (error == 1)
		return EXT_BREAK;

	return EXT_CONTINUE;
}