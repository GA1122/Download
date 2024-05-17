int kernel_sendmsg(struct socket *sock, struct msghdr *msg,
		   struct kvec *vec, size_t num, size_t size)
{
	mm_segment_t oldfs = get_fs();
	int result;

	set_fs(KERNEL_DS);
	 
	iov_iter_init(&msg->msg_iter, WRITE, (struct iovec *)vec, num, size);
	result = sock_sendmsg(sock, msg, size);
	set_fs(oldfs);
	return result;
}