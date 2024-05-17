 static long pipe_set_size(struct pipe_inode_info *pipe, unsigned long nr_pages)
 {
 	struct pipe_buffer *bufs;
 
 	 
 	if (nr_pages < pipe->nrbufs)
 		return -EBUSY;
 
 	bufs = kcalloc(nr_pages, sizeof(*bufs), GFP_KERNEL | __GFP_NOWARN);
 	if (unlikely(!bufs))
 		return -ENOMEM;
 
 	 
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
  
// 	account_pipe_buffers(pipe, pipe->buffers, nr_pages);
  	pipe->curbuf = 0;
  	kfree(pipe->bufs);
  	pipe->bufs = bufs;
 	pipe->buffers = nr_pages;
 	return nr_pages * PAGE_SIZE;
 }