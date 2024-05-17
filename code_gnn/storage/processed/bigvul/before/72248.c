input_userauth_info_req(int type, u_int32_t seq, void *ctxt)
{
	Authctxt *authctxt = ctxt;
	char *name, *inst, *lang, *prompt, *response;
	u_int num_prompts, i;
	int echo = 0;

	debug2("input_userauth_info_req");

	if (authctxt == NULL)
		fatal("input_userauth_info_req: no authentication context");

	authctxt->info_req_seen = 1;

	name = packet_get_string(NULL);
	inst = packet_get_string(NULL);
	lang = packet_get_string(NULL);
	if (strlen(name) > 0)
		logit("%s", name);
	if (strlen(inst) > 0)
		logit("%s", inst);
	free(name);
	free(inst);
	free(lang);

	num_prompts = packet_get_int();
	 
	packet_start(SSH2_MSG_USERAUTH_INFO_RESPONSE);
	packet_put_int(num_prompts);

	debug2("input_userauth_info_req: num_prompts %d", num_prompts);
	for (i = 0; i < num_prompts; i++) {
		prompt = packet_get_string(NULL);
		echo = packet_get_char();

		response = read_passphrase(prompt, echo ? RP_ECHO : 0);

		packet_put_cstring(response);
		explicit_bzero(response, strlen(response));
		free(response);
		free(prompt);
	}
	packet_check_eom();  

	packet_add_padding(64);
	packet_send();
	return 0;
}
