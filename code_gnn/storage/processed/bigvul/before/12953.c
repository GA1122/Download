ssh_packet_disconnect(struct ssh *ssh, const char *fmt,...)
{
	char buf[1024];
	va_list args;
	static int disconnecting = 0;
	int r;

	if (disconnecting)	 
		fatal("packet_disconnect called recursively.");
	disconnecting = 1;

	 
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	 
	logit("Disconnecting: %.100s", buf);

	 
	if ((r = sshpkt_disconnect(ssh, "%s", buf)) != 0)
		sshpkt_fatal(ssh, __func__, r);

	if ((r = ssh_packet_write_wait(ssh)) != 0)
		sshpkt_fatal(ssh, __func__, r);

	 
	ssh_packet_close(ssh);
	cleanup_exit(255);
}
