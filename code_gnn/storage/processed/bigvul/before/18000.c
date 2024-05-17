sshpkt_fatal(struct ssh *ssh, const char *tag, int r)
{
	switch (r) {
	case SSH_ERR_CONN_CLOSED:
		logdie("Connection closed by %.200s port %d",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
	case SSH_ERR_CONN_TIMEOUT:
		logdie("Connection %s %.200s port %d timed out",
		    ssh->state->server_side ? "from" : "to",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
	case SSH_ERR_DISCONNECTED:
		logdie("Disconnected from %.200s port %d",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
	case SSH_ERR_SYSTEM_ERROR:
		if (errno == ECONNRESET)
			logdie("Connection reset by %.200s port %d",
			    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh));
		 
	case SSH_ERR_NO_CIPHER_ALG_MATCH:
	case SSH_ERR_NO_MAC_ALG_MATCH:
	case SSH_ERR_NO_COMPRESS_ALG_MATCH:
	case SSH_ERR_NO_KEX_ALG_MATCH:
	case SSH_ERR_NO_HOSTKEY_ALG_MATCH:
		if (ssh && ssh->kex && ssh->kex->failed_choice) {
			logdie("Unable to negotiate with %.200s port %d: %s. "
			    "Their offer: %s", ssh_remote_ipaddr(ssh),
			    ssh_remote_port(ssh), ssh_err(r),
			    ssh->kex->failed_choice);
		}
		 
	default:
		logdie("%s%sConnection %s %.200s port %d: %s",
		    tag != NULL ? tag : "", tag != NULL ? ": " : "",
		    ssh->state->server_side ? "from" : "to",
		    ssh_remote_ipaddr(ssh), ssh_remote_port(ssh), ssh_err(r));
	}
}
