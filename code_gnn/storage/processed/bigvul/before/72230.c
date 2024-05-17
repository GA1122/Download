process_queued_listen_addrs(ServerOptions *options)
{
	u_int i;

	if (options->num_ports == 0)
		options->ports[options->num_ports++] = SSH_DEFAULT_PORT;
	if (options->address_family == -1)
		options->address_family = AF_UNSPEC;

	for (i = 0; i < options->num_queued_listens; i++) {
		add_listen_addr(options, options->queued_listen_addrs[i],
		    options->queued_listen_ports[i]);
		free(options->queued_listen_addrs[i]);
		options->queued_listen_addrs[i] = NULL;
	}
	free(options->queued_listen_addrs);
	options->queued_listen_addrs = NULL;
	free(options->queued_listen_ports);
	options->queued_listen_ports = NULL;
	options->num_queued_listens = 0;
}
