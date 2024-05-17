  static void pipe_advance(struct iov_iter *i, size_t size)
  {
  	struct pipe_inode_info *pipe = i->pipe;
	struct pipe_buffer *buf;
	int idx = i->idx;
	size_t off = i->iov_offset, orig_sz;
  	if (unlikely(i->count < size))
  		size = i->count;
	orig_sz = size;
  	if (size) {
// 		struct pipe_buffer *buf;
// 		size_t off = i->iov_offset, left = size;
// 		int idx = i->idx;
  		if (off)  
			size += off - pipe->bufs[idx].offset;
// 			left += off - pipe->bufs[idx].offset;
  		while (1) {
  			buf = &pipe->bufs[idx];
			if (size <= buf->len)
// 			if (left <= buf->len)
  				break;
			size -= buf->len;
// 			left -= buf->len;
  			idx = next_idx(idx, pipe);
  		}
		buf->len = size;
  		i->idx = idx;
		off = i->iov_offset = buf->offset + size;
	}
	if (off)
		idx = next_idx(idx, pipe);
	if (pipe->nrbufs) {
		int unused = (pipe->curbuf + pipe->nrbufs) & (pipe->buffers - 1);
		 
		while (idx != unused) {
			pipe_buf_release(pipe, &pipe->bufs[idx]);
			idx = next_idx(idx, pipe);
			pipe->nrbufs--;
		}
// 		i->iov_offset = buf->offset + left;
  	}
	i->count -= orig_sz;
// 	i->count -= size;
// 	 
// 	pipe_truncate(i);
  }