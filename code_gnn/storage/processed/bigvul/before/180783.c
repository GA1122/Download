 static ssize_t aio_setup_iocb(struct kiocb *kiocb, bool compat)
 {
 	struct file *file = kiocb->ki_filp;
 	ssize_t ret = 0;
 
 	switch (kiocb->ki_opcode) {
 	case IOCB_CMD_PREAD:
 		ret = -EBADF;
 		if (unlikely(!(file->f_mode & FMODE_READ)))
 			break;
 		ret = -EFAULT;
  		if (unlikely(!access_ok(VERIFY_WRITE, kiocb->ki_buf,
  			kiocb->ki_left)))
  			break;
		ret = security_file_permission(file, MAY_READ);
		if (unlikely(ret))
			break;
		ret = aio_setup_single_vector(kiocb);
// 		ret = aio_setup_single_vector(READ, file, kiocb);
  		if (ret)
  			break;
  		ret = -EINVAL;
 		if (file->f_op->aio_read)
 			kiocb->ki_retry = aio_rw_vect_retry;
 		break;
 	case IOCB_CMD_PWRITE:
 		ret = -EBADF;
 		if (unlikely(!(file->f_mode & FMODE_WRITE)))
 			break;
 		ret = -EFAULT;
  		if (unlikely(!access_ok(VERIFY_READ, kiocb->ki_buf,
  			kiocb->ki_left)))
  			break;
		ret = security_file_permission(file, MAY_WRITE);
		if (unlikely(ret))
			break;
		ret = aio_setup_single_vector(kiocb);
// 		ret = aio_setup_single_vector(WRITE, file, kiocb);
  		if (ret)
  			break;
  		ret = -EINVAL;
 		if (file->f_op->aio_write)
 			kiocb->ki_retry = aio_rw_vect_retry;
 		break;
 	case IOCB_CMD_PREADV:
  		ret = -EBADF;
  		if (unlikely(!(file->f_mode & FMODE_READ)))
  			break;
		ret = security_file_permission(file, MAY_READ);
		if (unlikely(ret))
			break;
  		ret = aio_setup_vectored_rw(READ, kiocb, compat);
  		if (ret)
  			break;
 		ret = -EINVAL;
 		if (file->f_op->aio_read)
 			kiocb->ki_retry = aio_rw_vect_retry;
 		break;
 	case IOCB_CMD_PWRITEV:
  		ret = -EBADF;
  		if (unlikely(!(file->f_mode & FMODE_WRITE)))
  			break;
		ret = security_file_permission(file, MAY_WRITE);
		if (unlikely(ret))
			break;
  		ret = aio_setup_vectored_rw(WRITE, kiocb, compat);
  		if (ret)
  			break;
 		ret = -EINVAL;
 		if (file->f_op->aio_write)
 			kiocb->ki_retry = aio_rw_vect_retry;
 		break;
 	case IOCB_CMD_FDSYNC:
 		ret = -EINVAL;
 		if (file->f_op->aio_fsync)
 			kiocb->ki_retry = aio_fdsync;
 		break;
 	case IOCB_CMD_FSYNC:
 		ret = -EINVAL;
 		if (file->f_op->aio_fsync)
 			kiocb->ki_retry = aio_fsync;
 		break;
 	default:
 		dprintk("EINVAL: io_submit: no operation provided\n");
 		ret = -EINVAL;
 	}
 
 	if (!kiocb->ki_retry)
 		return ret;
 
 	return 0;
 }