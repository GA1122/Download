add_listen_addr(ServerOptions *options, char *addr, int port)
{
	u_int i;

	if (port == 0)
		for (i = 0; i < options->num_ports; i++)
			add_one_listen_addr(options, addr, options->ports[i]);
	else
		add_one_listen_addr(options, addr, port);
}
