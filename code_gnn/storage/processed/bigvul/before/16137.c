GahpServer::command_initialize_from_file(const char *proxy_path,
										 const char *command)
{

	ASSERT(proxy_path);		 

	std::string buf;
	if ( command == NULL ) {
		command = "INITIALIZE_FROM_FILE";
	}
	int x = sprintf(buf,"%s %s",command,
					 escapeGahpString(proxy_path));
	ASSERT( x > 0 );
	write_line(buf.c_str());

	Gahp_Args result;
	read_argv(result);
	if ( result.argc == 0 || result.argv[0][0] != 'S' ) {
		char *reason;
		if ( result.argc > 1 ) {
			reason = result.argv[1];
		} else {
			reason = "Unspecified error";
		}
		dprintf(D_ALWAYS,"GAHP command '%s' failed: %s\n",command,reason);
		return false;
	}

	return true;
}
