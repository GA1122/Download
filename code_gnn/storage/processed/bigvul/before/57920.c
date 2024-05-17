int kernel_recvmsg(struct socket *sock, struct msghdr *msg,
		   struct kvec *vec, size_t num, size_t size, int flags)
{
	mm_segment_t oldfs = get_fs();
	int result;

	set_fs(KERNEL_DS);
	 
	iov_iter_init(&msg->msg_iter, READ, (struct iovec *)vec, num, size);
	result = sock_recvmsg(sock, msg, size, flags);
	set_fs(oldfs);
	return result;
}