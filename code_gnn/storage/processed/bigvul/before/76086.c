vrrp_register_workers(list l)
{
	sock_t *sock;
	timeval_t timer;
	element e;

	 
	memset(&timer, 0, sizeof(timer));

	 
	vrrp_init_state(vrrp_data->vrrp);

	 
	vrrp_init_sands(vrrp_data->vrrp);

	 
	if (!LIST_ISEMPTY(vrrp_data->vrrp_script))
		vrrp_init_script(vrrp_data->vrrp_script);

#ifdef _WITH_BFD_
	if (!LIST_ISEMPTY(vrrp_data->vrrp)) {
		 
		bfd_thread = thread_add_read(master, vrrp_bfd_thread, NULL,
					     bfd_vrrp_event_pipe[0], TIMER_NEVER);
	}
#endif

	 
	LIST_FOREACH(l, sock, e) {
		 
		if (sock->fd_in != -1)
			sock->thread = thread_add_read_sands(master, vrrp_read_dispatcher_thread,
						       sock, sock->fd_in, vrrp_compute_timer(sock));
	}
}
