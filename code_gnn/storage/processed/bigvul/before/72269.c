userauth_passwd(Authctxt *authctxt)
{
	static int attempt = 0;
	char prompt[150];
	char *password;
	const char *host = options.host_key_alias ?  options.host_key_alias :
	    authctxt->host;

	if (attempt++ >= options.number_of_password_prompts)
		return 0;

	if (attempt != 1)
		error("Permission denied, please try again.");

	snprintf(prompt, sizeof(prompt), "%.30s@%.128s's password: ",
	    authctxt->server_user, host);
	password = read_passphrase(prompt, 0);
	packet_start(SSH2_MSG_USERAUTH_REQUEST);
	packet_put_cstring(authctxt->server_user);
	packet_put_cstring(authctxt->service);
	packet_put_cstring(authctxt->method->name);
	packet_put_char(0);
	packet_put_cstring(password);
	explicit_bzero(password, strlen(password));
	free(password);
	packet_add_padding(64);
	packet_send();

	dispatch_set(SSH2_MSG_USERAUTH_PASSWD_CHANGEREQ,
	    &input_userauth_passwd_changereq);

	return 1;
}
