mux_client_request_alive(int fd)
{
	Buffer m;
	char *e;
	u_int pid, type, rid;

	debug3("%s: entering", __func__);

	buffer_init(&m);
	buffer_put_int(&m, MUX_C_ALIVE_CHECK);
	buffer_put_int(&m, muxclient_request_id);

	if (mux_client_write_packet(fd, &m) != 0)
		fatal("%s: write packet: %s", __func__, strerror(errno));

	buffer_clear(&m);

	 
	if (mux_client_read_packet(fd, &m) != 0) {
		buffer_free(&m);
		return 0;
	}

	type = buffer_get_int(&m);
	if (type != MUX_S_ALIVE) {
		e = buffer_get_string(&m, NULL);
		fatal("%s: master returned error: %s", __func__, e);
	}

	if ((rid = buffer_get_int(&m)) != muxclient_request_id)
		fatal("%s: out of sequence reply: my id %u theirs %u",
		    __func__, muxclient_request_id, rid);
	pid = buffer_get_int(&m);
	buffer_free(&m);

	debug3("%s: done pid = %u", __func__, pid);

	muxclient_request_id++;

	return pid;
}
