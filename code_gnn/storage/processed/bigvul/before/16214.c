GahpClient::nordugrid_ldap_query(const char *hostname, const char *ldap_base,
								 const char *ldap_filter,
								 const char *ldap_attrs, StringList &results)
{
	static const char* command = "NORDUGRID_LDAP_QUERY";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!hostname) hostname=NULLSTRING;
	if (!ldap_base) ldap_base=NULLSTRING;
	if (!ldap_filter) ldap_filter=NULLSTRING;
	if (!ldap_attrs) ldap_attrs=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(hostname) );
	char *esc2 = strdup( escapeGahpString(ldap_base) );
	char *esc3 = strdup( escapeGahpString(ldap_filter) );
	char *esc4 = strdup( escapeGahpString(ldap_attrs) );
	int x = sprintf(reqline,"%s %s %s %s", esc1, esc2, esc3, esc4 );
	free( esc1 );
	free( esc2 );
	free( esc3 );
	free( esc4 );
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
		if (result->argc < 3) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			error_string = result->argv[2];
		} else {
			error_string = "";
		}
		for ( int i = 3; i < result->argc; i++ ) {
			results.append( result->argv[i] );
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
