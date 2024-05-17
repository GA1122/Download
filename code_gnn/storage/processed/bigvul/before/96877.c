static ssize_t default_file_splice_read(struct file *in, loff_t *ppos,
				 struct pipe_inode_info *pipe, size_t len,
				 unsigned int flags)
{
	struct kvec *vec, __vec[PIPE_DEF_BUFFERS];
	struct iov_iter to;
	struct page **pages;
	unsigned int nr_pages;
	size_t offset, base, copied = 0;
	ssize_t res;
	int i;

	if (pipe->nrbufs == pipe->buffers)
		return -EAGAIN;

	 
	offset = *ppos & ~PAGE_MASK;

	iov_iter_pipe(&to, READ, pipe, len + offset);

	res = iov_iter_get_pages_alloc(&to, &pages, len + offset, &base);
	if (res <= 0)
		return -ENOMEM;

	nr_pages = DIV_ROUND_UP(res + base, PAGE_SIZE);

	vec = __vec;
	if (nr_pages > PIPE_DEF_BUFFERS) {
		vec = kmalloc_array(nr_pages, sizeof(struct kvec), GFP_KERNEL);
		if (unlikely(!vec)) {
			res = -ENOMEM;
			goto out;
		}
	}

	pipe->bufs[to.idx].offset = offset;
	pipe->bufs[to.idx].len -= offset;

	for (i = 0; i < nr_pages; i++) {
		size_t this_len = min_t(size_t, len, PAGE_SIZE - offset);
		vec[i].iov_base = page_address(pages[i]) + offset;
		vec[i].iov_len = this_len;
		len -= this_len;
		offset = 0;
	}

	res = kernel_readv(in, vec, nr_pages, *ppos);
	if (res > 0) {
		copied = res;
		*ppos += res;
	}

	if (vec != __vec)
		kfree(vec);
out:
	for (i = 0; i < nr_pages; i++)
		put_page(pages[i]);
	kvfree(pages);
	iov_iter_advance(&to, copied);	 
	return res;
}