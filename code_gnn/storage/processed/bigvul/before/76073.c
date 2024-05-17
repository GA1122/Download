vrrp_dispatcher_init(__attribute__((unused)) thread_t * thread)
{
	vrrp_create_sockpool(vrrp_data->vrrp_socket_pool);

	 
	vrrp_open_sockpool(vrrp_data->vrrp_socket_pool);

	 
	vrrp_set_fds(vrrp_data->vrrp_socket_pool);

	 
	 
	vrrp_register_workers(vrrp_data->vrrp_socket_pool);

	 
	if (__test_bit(LOG_DETAIL_BIT, &debug))
		dump_list(NULL, vrrp_data->vrrp_socket_pool);

	vrrp_initialised = true;

	return 1;
}
