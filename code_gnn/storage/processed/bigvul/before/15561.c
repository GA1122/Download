client_cleanup_stdio_fwd(int id, void *arg)
{
	debug("stdio forwarding: done");
	cleanup_exit(0);
}
