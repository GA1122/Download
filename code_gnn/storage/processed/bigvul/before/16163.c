GahpClient::cream_set_lease(const char *service, const char *lease_id, time_t &lease_expiry)
{
	static const char* command = "CREAM_SET_LEASE";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}
	if (!service) service=NULLSTRING;
	if (!lease_id) lease_id=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(service) );
	char *esc2 = strdup( escapeGahpString(lease_id) );
	int x = sprintf(reqline, "%s %s %ld", esc1, esc2, (long)lease_expiry);
	free( esc1 );
	free( esc2 );
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,normal_proxy,high_prio);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		int rc = 0;
		if ( result->argc == 2 ) {
			if ( !strcmp( result->argv[1], NULLSTRING ) ) {
				EXCEPT( "Bad %s result", command );
			}
			error_string = result->argv[1];
			rc = 1;
		} else if ( result->argc == 3 ) {
			if ( strcmp( result->argv[1], NULLSTRING ) ) {
				EXCEPT( "Bad %s result", command );
			}

			if ( strcasecmp(result->argv[2], NULLSTRING) ) {
				lease_expiry = atoi( result->argv[2] );
			}
			rc = 0;
		} else {
			EXCEPT( "Bad %s result", command );
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
