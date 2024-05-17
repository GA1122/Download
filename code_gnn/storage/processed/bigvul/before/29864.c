standard_receive3(struct TCP_Server_Info *server, struct mid_q_entry *mid)
{
	int length;
	char *buf = server->smallbuf;
	unsigned int pdu_length = get_rfc1002_length(buf);

	 
	if (pdu_length > CIFSMaxBufSize + MAX_HEADER_SIZE(server) - 4) {
		cifs_dbg(VFS, "SMB response too long (%u bytes)\n", pdu_length);
		cifs_reconnect(server);
		wake_up(&server->response_q);
		return -EAGAIN;
	}

	 
	if (pdu_length > MAX_CIFS_SMALL_BUFFER_SIZE - 4) {
		server->large_buf = true;
		memcpy(server->bigbuf, buf, server->total_read);
		buf = server->bigbuf;
	}

	 
	length = cifs_read_from_socket(server, buf + HEADER_SIZE(server) - 1,
				pdu_length - HEADER_SIZE(server) + 1 + 4);
	if (length < 0)
		return length;
	server->total_read += length;

	dump_smb(buf, server->total_read);

	 
	length = server->ops->check_message(buf, server->total_read);
	if (length != 0)
		cifs_dump_mem("Bad SMB: ", buf,
			min_t(unsigned int, server->total_read, 48));

	if (server->ops->is_status_pending &&
	    server->ops->is_status_pending(buf, server, length))
		return -1;

	if (!mid)
		return length;

	handle_mid(mid, server, buf, length);
	return 0;
}
