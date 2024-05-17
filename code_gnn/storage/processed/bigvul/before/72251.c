input_userauth_service_accept(int type, u_int32_t seqnr, void *ctxt)
{
	Authctxt *authctxt = ctxt;
	struct ssh *ssh = active_state;
	int r;

	if (ssh_packet_remaining(ssh) > 0) {
		char *reply;

		if ((r = sshpkt_get_cstring(ssh, &reply, NULL)) != 0)
			goto out;
		debug2("service_accept: %s", reply);
		free(reply);
	} else {
		debug2("buggy server: service_accept w/o service");
	}
	if ((r = sshpkt_get_end(ssh)) != 0)
		goto out;
	debug("SSH2_MSG_SERVICE_ACCEPT received");

	 
	userauth_none(authctxt);

	ssh_dispatch_set(ssh, SSH2_MSG_EXT_INFO, &input_userauth_error);
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_SUCCESS, &input_userauth_success);
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_FAILURE, &input_userauth_failure);
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_BANNER, &input_userauth_banner);
	r = 0;
 out:
	return r;
}
