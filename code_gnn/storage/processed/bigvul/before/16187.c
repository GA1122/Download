GahpClient::globus_gass_server_superez_init( char **gass_url, int port )
{

	static const char* command = "GASS_SERVER_INIT";

	if ( server->globus_gass_server_url != NULL ) {
		*gass_url = strdup( server->globus_gass_server_url );
		return 0;
	}

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	std::string reqline;
	int x = sprintf(reqline,"%d",port);
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc != 3) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			*gass_url = strdup(result->argv[2]);
			server->globus_gass_server_url = strdup(result->argv[2]);
		}
		delete result;
		return rc;
	}

	if ( check_pending_timeout(command,buf) ) {
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
