 static ssize_t aio_setup_single_vector(struct kiocb *kiocb)
// static ssize_t aio_setup_single_vector(int type, struct file * file, struct kiocb *kiocb)
  {
// 	int bytes;
// 
// 	bytes = rw_verify_area(type, file, &kiocb->ki_pos, kiocb->ki_left);
// 	if (bytes < 0)
// 		return bytes;
// 
  	kiocb->ki_iovec = &kiocb->ki_inline_vec;
  	kiocb->ki_iovec->iov_base = kiocb->ki_buf;
	kiocb->ki_iovec->iov_len = kiocb->ki_left;
// 	kiocb->ki_iovec->iov_len = bytes;
  	kiocb->ki_nr_segs = 1;
  	kiocb->ki_cur_seg = 0;
  	return 0;
 }