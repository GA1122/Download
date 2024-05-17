static long pipe_set_size(struct pipe_inode_info *pipe, unsigned long arg)
{
	struct pipe_buffer *bufs;
	unsigned int size, nr_pages;
	unsigned long user_bufs;
	long ret = 0;

	size = round_pipe_size(arg);
	nr_pages = size >> PAGE_SHIFT;

	if (!nr_pages)
		return -EINVAL;

	 
	if (nr_pages > pipe->buffers &&
			size > pipe_max_size && !capable(CAP_SYS_RESOURCE))
		return -EPERM;

	user_bufs = account_pipe_buffers(pipe->user, pipe->buffers, nr_pages);

	if (nr_pages > pipe->buffers &&
			(too_many_pipe_buffers_hard(user_bufs) ||
			 too_many_pipe_buffers_soft(user_bufs)) &&
			is_unprivileged_user()) {
		ret = -EPERM;
		goto out_revert_acct;
	}

	 
	if (nr_pages < pipe->nrbufs) {
		ret = -EBUSY;
		goto out_revert_acct;
	}

	bufs = kcalloc(nr_pages, sizeof(*bufs),
		       GFP_KERNEL_ACCOUNT | __GFP_NOWARN);
	if (unlikely(!bufs)) {
		ret = -ENOMEM;
		goto out_revert_acct;
	}

	 
	if (pipe->nrbufs) {
		unsigned int tail;
		unsigned int head;

		tail = pipe->curbuf + pipe->nrbufs;
		if (tail < pipe->buffers)
			tail = 0;
		else
			tail &= (pipe->buffers - 1);

		head = pipe->nrbufs - tail;
		if (head)
			memcpy(bufs, pipe->bufs + pipe->curbuf, head * sizeof(struct pipe_buffer));
		if (tail)
			memcpy(bufs + head, pipe->bufs, tail * sizeof(struct pipe_buffer));
	}

	pipe->curbuf = 0;
	kfree(pipe->bufs);
	pipe->bufs = bufs;
	pipe->buffers = nr_pages;
	return nr_pages * PAGE_SIZE;

out_revert_acct:
	(void) account_pipe_buffers(pipe->user, nr_pages, pipe->buffers);
	return ret;
}