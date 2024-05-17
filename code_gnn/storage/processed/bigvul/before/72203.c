ssh_packet_write_poll(struct ssh *ssh)
{
	struct session_state *state = ssh->state;
	int len = sshbuf_len(state->output);
	int r;

	if (len > 0) {
		len = write(state->connection_out,
		    sshbuf_ptr(state->output), len);
		if (len == -1) {
			if (errno == EINTR || errno == EAGAIN)
				return 0;
			return SSH_ERR_SYSTEM_ERROR;
		}
		if (len == 0)
			return SSH_ERR_CONN_CLOSED;
		if ((r = sshbuf_consume(state->output, len)) != 0)
			return r;
	}
	return 0;
}
