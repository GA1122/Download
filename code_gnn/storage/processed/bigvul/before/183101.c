 pipe_read(struct kiocb *iocb, const struct iovec *_iov,
 	   unsigned long nr_segs, loff_t pos)
 {
 	struct file *filp = iocb->ki_filp;
 	struct pipe_inode_info *pipe = filp->private_data;
 	int do_wakeup;
  	ssize_t ret;
  	struct iovec *iov = (struct iovec *)_iov;
  	size_t total_len;
// 	struct iov_iter iter;
  
  	total_len = iov_length(iov, nr_segs);
  	 
  	if (unlikely(total_len == 0))
  		return 0;
  
// 	iov_iter_init(&iter, iov, nr_segs, total_len, 0);
// 
  	do_wakeup = 0;
  	ret = 0;
  	__pipe_lock(pipe);
 	for (;;) {
 		int bufs = pipe->nrbufs;
 		if (bufs) {
  			int curbuf = pipe->curbuf;
  			struct pipe_buffer *buf = pipe->bufs + curbuf;
  			const struct pipe_buf_operations *ops = buf->ops;
			void *addr;
  			size_t chars = buf->len;
			int error, atomic;
// 			size_t written;
// 			int error;
  
  			if (chars > total_len)
  				chars = total_len;
 
 			error = ops->confirm(pipe, buf);
 			if (error) {
 				if (!ret)
 					ret = error;
  				break;
  			}
  
			atomic = !iov_fault_in_pages_write(iov, chars);
redo:
			if (atomic)
				addr = kmap_atomic(buf->page);
			else
				addr = kmap(buf->page);
			error = pipe_iov_copy_to_user(iov, addr + buf->offset, chars, atomic);
			if (atomic)
				kunmap_atomic(addr);
			else
				kunmap(buf->page);
			if (unlikely(error)) {
				 
				if (atomic) {
					atomic = 0;
					goto redo;
				}
// 			written = copy_page_to_iter(buf->page, buf->offset, chars, &iter);
// 			if (unlikely(written < chars)) {
  				if (!ret)
					ret = error;
// 					ret = -EFAULT;
  				break;
  			}
  			ret += chars;
 			buf->offset += chars;
 			buf->len -= chars;
 
 			 
 			if (buf->flags & PIPE_BUF_FLAG_PACKET) {
 				total_len = chars;
 				buf->len = 0;
 			}
 
 			if (!buf->len) {
 				buf->ops = NULL;
 				ops->release(pipe, buf);
 				curbuf = (curbuf + 1) & (pipe->buffers - 1);
 				pipe->curbuf = curbuf;
 				pipe->nrbufs = --bufs;
 				do_wakeup = 1;
 			}
 			total_len -= chars;
 			if (!total_len)
 				break;	 
 		}
 		if (bufs)	 
 			continue;
 		if (!pipe->writers)
 			break;
 		if (!pipe->waiting_writers) {
 			 
 			if (ret)
 				break;
 			if (filp->f_flags & O_NONBLOCK) {
 				ret = -EAGAIN;
 				break;
 			}
 		}
 		if (signal_pending(current)) {
 			if (!ret)
 				ret = -ERESTARTSYS;
 			break;
 		}
 		if (do_wakeup) {
 			wake_up_interruptible_sync_poll(&pipe->wait, POLLOUT | POLLWRNORM);
  			kill_fasync(&pipe->fasync_writers, SIGIO, POLL_OUT);
 		}
 		pipe_wait(pipe);
 	}
 	__pipe_unlock(pipe);
 
 	 
 	if (do_wakeup) {
 		wake_up_interruptible_sync_poll(&pipe->wait, POLLOUT | POLLWRNORM);
 		kill_fasync(&pipe->fasync_writers, SIGIO, POLL_OUT);
 	}
 	if (ret > 0)
 		file_accessed(filp);
 	return ret;
 }