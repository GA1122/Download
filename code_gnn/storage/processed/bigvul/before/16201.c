GahpClient::gt4_gram_client_delegate_credentials(const char *delegation_service_url, char ** delegation_uri)
{
	static const char* command = "GT4_DELEGATE_CREDENTIAL";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	ASSERT (delegation_service_url && *delegation_service_url);
	std::string reqline;
	int x = sprintf(reqline,"%s",escapeGahpString(delegation_service_url));
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,deleg_proxy);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc != 4) {
			EXCEPT("Bad %s Result",command);
		}

		int rc = atoi(result->argv[1]);
 
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			*delegation_uri = strdup(result->argv[2]);
		}

		if ( strcasecmp(result->argv[3], NULLSTRING) ) {
			error_string = result->argv[3];
		} else {
			error_string = "";
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
