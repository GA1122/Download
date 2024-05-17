sshpkt_get_end(struct ssh *ssh)
{
	if (sshbuf_len(ssh->state->incoming_packet) > 0)
		return SSH_ERR_UNEXPECTED_TRAILING_DATA;
	return 0;
}
