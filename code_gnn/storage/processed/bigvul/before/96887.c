ssize_t splice_direct_to_actor(struct file *in, struct splice_desc *sd,
			       splice_direct_actor *actor)
{
	struct pipe_inode_info *pipe;
	long ret, bytes;
	umode_t i_mode;
	size_t len;
	int i, flags, more;

	 
	i_mode = file_inode(in)->i_mode;
	if (unlikely(!S_ISREG(i_mode) && !S_ISBLK(i_mode)))
		return -EINVAL;

	 
	pipe = current->splice_pipe;
	if (unlikely(!pipe)) {
		pipe = alloc_pipe_info();
		if (!pipe)
			return -ENOMEM;

		 
		pipe->readers = 1;

		current->splice_pipe = pipe;
	}

	 
	ret = 0;
	bytes = 0;
	len = sd->total_len;
	flags = sd->flags;

	 
	sd->flags &= ~SPLICE_F_NONBLOCK;
	more = sd->flags & SPLICE_F_MORE;

	WARN_ON_ONCE(pipe->nrbufs != 0);

	while (len) {
		size_t read_len;
		loff_t pos = sd->pos, prev_pos = pos;

		 
		read_len = min_t(size_t, len,
				 (pipe->buffers - pipe->nrbufs) << PAGE_SHIFT);
		ret = do_splice_to(in, &pos, pipe, read_len, flags);
		if (unlikely(ret <= 0))
			goto out_release;

		read_len = ret;
		sd->total_len = read_len;

		 
		if (read_len < len)
			sd->flags |= SPLICE_F_MORE;
		else if (!more)
			sd->flags &= ~SPLICE_F_MORE;
		 
		ret = actor(pipe, sd);
		if (unlikely(ret <= 0)) {
			sd->pos = prev_pos;
			goto out_release;
		}

		bytes += ret;
		len -= ret;
		sd->pos = pos;

		if (ret < read_len) {
			sd->pos = prev_pos + ret;
			goto out_release;
		}
	}

done:
	pipe->nrbufs = pipe->curbuf = 0;
	file_accessed(in);
	return bytes;

out_release:
	 
	for (i = 0; i < pipe->buffers; i++) {
		struct pipe_buffer *buf = pipe->bufs + i;

		if (buf->ops)
			pipe_buf_release(pipe, buf);
	}

	if (!bytes)
		bytes = ret;

	goto done;
}
