GahpClient::globus_gram_client_error_string(int error_code)
{
	static char buf[200];
	static const char* command = "GRAM_ERROR_STRING";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		strcpy(buf,"Unknown error");
		return buf;
	}

	int x = snprintf(buf,sizeof(buf),"%s %d",command,error_code);
	ASSERT( x > 0 && x < (int)sizeof(buf) );
	server->write_line(buf);
	Gahp_Args result;
	server->read_argv(result);
	if ( result.argc < 2 || result.argv[0][0] != 'S' ) {
		dprintf(D_ALWAYS,"GAHP command '%s' failed: error_code=%d\n",
						command,error_code);
		return NULL;
	}
	strncpy(buf,result.argv[1],sizeof(buf)-1);
	buf[sizeof(buf)-1] = '\0';

	return buf;
}
