vrrp_read_dispatcher_thread(thread_t * thread)
{
	sock_t *sock;
	int fd;

	 
	sock = THREAD_ARG(thread);

	 
	if (thread->type == THREAD_READ_TIMEOUT || sock->fd_in == -1)
		fd = vrrp_dispatcher_read_timeout(sock);
	else
		fd = vrrp_dispatcher_read(sock);

	 
	if (fd != -1)
		sock->thread = thread_add_read_sands(thread->master, vrrp_read_dispatcher_thread,
					       sock, fd, vrrp_compute_timer(sock));

	return 0;
}
