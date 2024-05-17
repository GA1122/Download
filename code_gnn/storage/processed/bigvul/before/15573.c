ssh_stdio_confirm(int id, int success, void *arg)
{
	if (!success)
		fatal("stdio forwarding failed");
}
