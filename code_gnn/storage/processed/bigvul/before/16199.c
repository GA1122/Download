GahpClient::gt4_generate_submit_id (char ** submit_id)
{
	static const char * command = "GT4_GENERATE_SUBMIT_ID";

	if ( submit_id ) {
		*submit_id = NULL;
	}

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if ( !is_pending( command, NULL ) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending( command, NULL, normal_proxy );
	}


	Gahp_Args* result = get_pending_result( command, NULL );
	if ( result ) {
		if (result->argc != 2) {
			EXCEPT( "Bad %s Result", command );
		}
		if ( strcasecmp(result->argv[1], NULLSTRING) ) {
			*submit_id = strdup( result->argv[1] );
		} else {
			*submit_id = NULL;
		}
		delete result;
		return 0;
	}

	if ( check_pending_timeout( command, NULL ) ) {
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
