ssh_packet_read_seqnr(struct ssh *ssh, u_char *typep, u_int32_t *seqnr_p)
{
	struct session_state *state = ssh->state;
	int len, r, ms_remain;
	fd_set *setp;
	char buf[8192];
	struct timeval timeout, start, *timeoutp = NULL;

	DBG(debug("packet_read()"));

	setp = calloc(howmany(state->connection_in + 1,
	    NFDBITS), sizeof(fd_mask));
	if (setp == NULL)
		return SSH_ERR_ALLOC_FAIL;

	 
	if ((r = ssh_packet_write_wait(ssh)) != 0)
		goto out;

	 
	for (;;) {
		 
		r = ssh_packet_read_poll_seqnr(ssh, typep, seqnr_p);
		if (r != 0)
			break;
		if (!compat20 && (
		    *typep == SSH_SMSG_SUCCESS
		    || *typep == SSH_SMSG_FAILURE
		    || *typep == SSH_CMSG_EOF
		    || *typep == SSH_CMSG_EXIT_CONFIRMATION))
			if ((r = sshpkt_get_end(ssh)) != 0)
				break;
		 
		if (*typep != SSH_MSG_NONE)
			break;
		 
		memset(setp, 0, howmany(state->connection_in + 1,
		    NFDBITS) * sizeof(fd_mask));
		FD_SET(state->connection_in, setp);

		if (state->packet_timeout_ms > 0) {
			ms_remain = state->packet_timeout_ms;
			timeoutp = &timeout;
		}
		 
		for (;;) {
			if (state->packet_timeout_ms != -1) {
				ms_to_timeval(&timeout, ms_remain);
				gettimeofday(&start, NULL);
			}
			if ((r = select(state->connection_in + 1, setp,
			    NULL, NULL, timeoutp)) >= 0)
				break;
			if (errno != EAGAIN && errno != EINTR)
				break;
			if (state->packet_timeout_ms == -1)
				continue;
			ms_subtract_diff(&start, &ms_remain);
			if (ms_remain <= 0) {
				r = 0;
				break;
			}
		}
		if (r == 0)
			return SSH_ERR_CONN_TIMEOUT;
		 
		len = read(state->connection_in, buf, sizeof(buf));
		if (len == 0) {
			r = SSH_ERR_CONN_CLOSED;
			goto out;
		}
		if (len < 0) {
			r = SSH_ERR_SYSTEM_ERROR;
			goto out;
		}

		 
		if ((r = ssh_packet_process_incoming(ssh, buf, len)) != 0)
			goto out;
	}
 out:
	free(setp);
	return r;
}