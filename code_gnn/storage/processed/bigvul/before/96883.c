iter_file_splice_write(struct pipe_inode_info *pipe, struct file *out,
			  loff_t *ppos, size_t len, unsigned int flags)
{
	struct splice_desc sd = {
		.total_len = len,
		.flags = flags,
		.pos = *ppos,
		.u.file = out,
	};
	int nbufs = pipe->buffers;
	struct bio_vec *array = kcalloc(nbufs, sizeof(struct bio_vec),
					GFP_KERNEL);
	ssize_t ret;

	if (unlikely(!array))
		return -ENOMEM;

	pipe_lock(pipe);

	splice_from_pipe_begin(&sd);
	while (sd.total_len) {
		struct iov_iter from;
		size_t left;
		int n, idx;

		ret = splice_from_pipe_next(pipe, &sd);
		if (ret <= 0)
			break;

		if (unlikely(nbufs < pipe->buffers)) {
			kfree(array);
			nbufs = pipe->buffers;
			array = kcalloc(nbufs, sizeof(struct bio_vec),
					GFP_KERNEL);
			if (!array) {
				ret = -ENOMEM;
				break;
			}
		}

		 
		left = sd.total_len;
		for (n = 0, idx = pipe->curbuf; left && n < pipe->nrbufs; n++, idx++) {
			struct pipe_buffer *buf = pipe->bufs + idx;
			size_t this_len = buf->len;

			if (this_len > left)
				this_len = left;

			if (idx == pipe->buffers - 1)
				idx = -1;

			ret = pipe_buf_confirm(pipe, buf);
			if (unlikely(ret)) {
				if (ret == -ENODATA)
					ret = 0;
				goto done;
			}

			array[n].bv_page = buf->page;
			array[n].bv_len = this_len;
			array[n].bv_offset = buf->offset;
			left -= this_len;
		}

		iov_iter_bvec(&from, WRITE, array, n, sd.total_len - left);
		ret = vfs_iter_write(out, &from, &sd.pos, 0);
		if (ret <= 0)
			break;

		sd.num_spliced += ret;
		sd.total_len -= ret;
		*ppos = sd.pos;

		 
		while (ret) {
			struct pipe_buffer *buf = pipe->bufs + pipe->curbuf;
			if (ret >= buf->len) {
				ret -= buf->len;
				buf->len = 0;
				pipe_buf_release(pipe, buf);
				pipe->curbuf = (pipe->curbuf + 1) & (pipe->buffers - 1);
				pipe->nrbufs--;
				if (pipe->files)
					sd.need_wakeup = true;
			} else {
				buf->offset += ret;
				buf->len -= ret;
				ret = 0;
			}
		}
	}
done:
	kfree(array);
	splice_from_pipe_end(pipe, &sd);

	pipe_unlock(pipe);

	if (sd.num_spliced)
		ret = sd.num_spliced;

	return ret;
}