int GahpClient::ec2_vm_create_keypair( const char *service_url, const char * publickeyfile, const char * privatekeyfile,
								   	      const char * keyname, const char * outputfile, char* & error_code)
{
	static const char* command = "EC2_VM_CREATE_KEYPAIR";
	
	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}
	
	if ( (service_url == NULL) || (publickeyfile == NULL) || (privatekeyfile == NULL) || (keyname == NULL) || (outputfile == NULL) ) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}
	
	std::string reqline;
	
	char* esc1 = strdup( escapeGahpString(service_url) );
	char* esc2 = strdup( escapeGahpString(publickeyfile) );
	char* esc3 = strdup( escapeGahpString(privatekeyfile) );
	char* esc4 = strdup( escapeGahpString(keyname) );
	char* esc5 = strdup( escapeGahpString(outputfile) );
	
	int x = sprintf(reqline, "%s %s %s %s %s", esc1, esc2, esc3, esc4, esc5);
	
	free( esc1 );
	free( esc2 );
	free( esc3 );
	free( esc4 );
	free( esc5 );

	ASSERT( x > 0 );
	
	const char *buf = reqline.c_str();
		
	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command, buf, deleg_proxy);
	}
	

	Gahp_Args* result = get_pending_result(command, buf);
	
		
	if ( result ) {
		int rc = 0;
		if (result->argc == 2) {
			rc = atoi(result->argv[1]);
			if (rc == 1) {
				error_string = "";
			}
		} 
		else if ( result->argc == 4 ) {
			rc = atoi( result->argv[1] );
			error_code = strdup(result->argv[2]);
			error_string = result->argv[3];
		} else {
			EXCEPT( "Bad %s result", command );
		}

		delete result;
		return rc;
	}
	
	if ( check_pending_timeout(command, buf) ) 
	{
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;	
}	