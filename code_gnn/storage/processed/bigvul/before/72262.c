ssh_userauth2(const char *local_user, const char *server_user, char *host,
    Sensitive *sensitive)
{
	struct ssh *ssh = active_state;
	Authctxt authctxt;
	int r;

	if (options.challenge_response_authentication)
		options.kbd_interactive_authentication = 1;
	if (options.preferred_authentications == NULL)
		options.preferred_authentications = authmethods_get();

	 
	memset(&authctxt, 0, sizeof(authctxt));
	pubkey_prepare(&authctxt);
	authctxt.server_user = server_user;
	authctxt.local_user = local_user;
	authctxt.host = host;
	authctxt.service = "ssh-connection";		 
	authctxt.success = 0;
	authctxt.method = authmethod_lookup("none");
	authctxt.authlist = NULL;
	authctxt.methoddata = NULL;
	authctxt.sensitive = sensitive;
	authctxt.active_ktype = authctxt.oktypes = authctxt.ktypes = NULL;
	authctxt.info_req_seen = 0;
	authctxt.agent_fd = -1;
	if (authctxt.method == NULL)
		fatal("ssh_userauth2: internal error: cannot send userauth none request");

	if ((r = sshpkt_start(ssh, SSH2_MSG_SERVICE_REQUEST)) != 0 ||
	    (r = sshpkt_put_cstring(ssh, "ssh-userauth")) != 0 ||
	    (r = sshpkt_send(ssh)) != 0)
		fatal("%s: %s", __func__, ssh_err(r));

	ssh_dispatch_init(ssh, &input_userauth_error);
	ssh_dispatch_set(ssh, SSH2_MSG_EXT_INFO, &input_userauth_ext_info);
	ssh_dispatch_set(ssh, SSH2_MSG_SERVICE_ACCEPT, &input_userauth_service_accept);
	ssh_dispatch_run(ssh, DISPATCH_BLOCK, &authctxt.success, &authctxt);	 

	pubkey_cleanup(&authctxt);
	ssh_dispatch_range(ssh, SSH2_MSG_USERAUTH_MIN, SSH2_MSG_USERAUTH_MAX, NULL);

	if (!authctxt.success)
		fatal("Authentication failed.");
	debug("Authentication succeeded (%s).", authctxt.method->name);
}
