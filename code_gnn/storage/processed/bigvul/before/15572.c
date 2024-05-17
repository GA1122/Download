ssh_init_stdio_forwarding(void)
{
	Channel *c;
	int in, out;

	if (stdio_forward_host == NULL)
		return;
	if (!compat20)
		fatal("stdio forwarding require Protocol 2");

	debug3("%s: %s:%d", __func__, stdio_forward_host, stdio_forward_port);

	if ((in = dup(STDIN_FILENO)) < 0 ||
	    (out = dup(STDOUT_FILENO)) < 0)
		fatal("channel_connect_stdio_fwd: dup() in/out failed");
	if ((c = channel_connect_stdio_fwd(stdio_forward_host,
	    stdio_forward_port, in, out)) == NULL)
		fatal("%s: channel_connect_stdio_fwd failed", __func__);
	channel_register_cleanup(c->self, client_cleanup_stdio_fwd, 0);
	channel_register_open_confirm(c->self, ssh_stdio_confirm, NULL);
}
