static unsigned int sock_poll(struct file *file, poll_table *wait)
{
	unsigned int busy_flag = 0;
	struct socket *sock;

	 
	sock = file->private_data;

	if (sk_can_busy_loop(sock->sk)) {
		 
		busy_flag = POLL_BUSY_LOOP;

		 
		if (wait && (wait->_key & POLL_BUSY_LOOP))
			sk_busy_loop(sock->sk, 1);
	}

	return busy_flag | sock->ops->poll(file, sock, wait);
}
