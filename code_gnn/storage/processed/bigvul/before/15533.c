mux_client_hello_exchange(int fd)
{
	Buffer m;
	u_int type, ver;

	buffer_init(&m);
	buffer_put_int(&m, MUX_MSG_HELLO);
	buffer_put_int(&m, SSHMUX_VER);
	 

	if (mux_client_write_packet(fd, &m) != 0)
		fatal("%s: write packet: %s", __func__, strerror(errno));

	buffer_clear(&m);

	 
	if (mux_client_read_packet(fd, &m) != 0) {
		buffer_free(&m);
		return -1;
	}

	type = buffer_get_int(&m);
	if (type != MUX_MSG_HELLO)
		fatal("%s: expected HELLO (%u) received %u",
		    __func__, MUX_MSG_HELLO, type);
	ver = buffer_get_int(&m);
	if (ver != SSHMUX_VER)
		fatal("Unsupported multiplexing protocol version %d "
		    "(expected %d)", ver, SSHMUX_VER);
	debug2("%s: master version %u", __func__, ver);
	 
	while (buffer_len(&m) > 0) {
		char *name = buffer_get_string(&m, NULL);
		char *value = buffer_get_string(&m, NULL);

		debug2("Unrecognised master extension \"%s\"", name);
		free(name);
		free(value);
	}
	buffer_free(&m);
	return 0;
}
