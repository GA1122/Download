ssize_t generic_file_splice_read(struct file *in, loff_t *ppos,
				 struct pipe_inode_info *pipe, size_t len,
				 unsigned int flags)
{
	struct iov_iter to;
	struct kiocb kiocb;
	int idx, ret;

	iov_iter_pipe(&to, READ, pipe, len);
	idx = to.idx;
	init_sync_kiocb(&kiocb, in);
	kiocb.ki_pos = *ppos;
	ret = call_read_iter(in, &kiocb, &to);
	if (ret > 0) {
		*ppos = kiocb.ki_pos;
		file_accessed(in);
	} else if (ret < 0) {
		to.idx = idx;
		to.iov_offset = 0;
		iov_iter_advance(&to, 0);  
		 
		if (ret == -EFAULT)
			ret = -EAGAIN;
	}

	return ret;
}