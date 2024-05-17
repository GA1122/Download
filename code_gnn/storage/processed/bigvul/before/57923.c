static ssize_t sock_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct file *file = iocb->ki_filp;
	struct socket *sock = file->private_data;
	struct msghdr msg = {.msg_iter = *to};
	ssize_t res;

	if (file->f_flags & O_NONBLOCK)
		msg.msg_flags = MSG_DONTWAIT;

	if (iocb->ki_pos != 0)
		return -ESPIPE;

	if (iocb->ki_nbytes == 0)	 
		return 0;

	res = __sock_recvmsg(iocb, sock, &msg,
			     iocb->ki_nbytes, msg.msg_flags);
	*to = msg.msg_iter;
	return res;
}