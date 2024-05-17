GahpClient::gt4_gram_client_callback_allow(
	globus_gt4_gram_callback_func_t callback_func,
	void * user_callback_arg,
	char ** callback_contact)
{
	char buf[150];
	static const char* command = "GT4_GRAM_CALLBACK_ALLOW";

	if (callback_contact) {
		*callback_contact = NULL;
	}
	if ( server->globus_gt4_gram_callback_contact ) {
		if ( callback_func != server->globus_gt4_gram_callback_func || 
			 user_callback_arg != server->globus_gt4_gram_user_callback_arg )
		{
			EXCEPT("gt4_gram_client_callback_allow called twice");
		}
		if (callback_contact) {
			*callback_contact = strdup(server->globus_gt4_gram_callback_contact);
			ASSERT(*callback_contact);
		}
		return 0;
	}

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if ( m_mode == results_only ) {
		return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
	}

	int reqid = server->new_reqid();
	int x = snprintf(buf,sizeof(buf),"%s %d",command,reqid);
	ASSERT( x > 0 && x < (int)sizeof(buf) );
	server->write_line(buf);
	Gahp_Args result;
	server->read_argv(result);
	if ( result.argc != 2 || result.argv[0][0] != 'S' ) {
		const char *es = result.argc >= 3 ? result.argv[2] : "???";
		dprintf(D_ALWAYS,"GAHP command '%s' failed: %s\n",
				command, es);
		if ( result.argc >= 3 && strcasecmp(result.argv[2], NULLSTRING) ) {
			error_string = result.argv[2];
		} else {
			error_string = "";
		}
		return 1;
	} 

	server->globus_gt4_gram_callback_reqid = reqid;
 	server->globus_gt4_gram_callback_func = callback_func;
	server->globus_gt4_gram_user_callback_arg = user_callback_arg;
	server->globus_gt4_gram_callback_contact = strdup(result.argv[1]);
	ASSERT(server->globus_gt4_gram_callback_contact);
	*callback_contact = strdup(server->globus_gt4_gram_callback_contact);
	ASSERT(*callback_contact);

	return 0;
}
