queue_listen_addr(ServerOptions *options, char *addr, int port)
{
	options->queued_listen_addrs = xreallocarray(
	    options->queued_listen_addrs, options->num_queued_listens + 1,
	    sizeof(addr));
	options->queued_listen_ports = xreallocarray(
	    options->queued_listen_ports, options->num_queued_listens + 1,
	    sizeof(port));
	options->queued_listen_addrs[options->num_queued_listens] =
	    xstrdup(addr);
	options->queued_listen_ports[options->num_queued_listens] = port;
	options->num_queued_listens++;
}
