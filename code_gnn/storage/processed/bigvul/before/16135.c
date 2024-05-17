GahpServer::command_cache_proxy_from_file( GahpProxyInfo *new_proxy )
{
	static const char *command = "CACHE_PROXY_FROM_FILE";

	ASSERT(new_proxy);		 

	if  (m_commands_supported->contains_anycase(command)==FALSE) {
		return false;
	}

	std::string buf;
	int x = sprintf(buf,"%s %d %s",command,new_proxy->proxy->id,
					 escapeGahpString(new_proxy->proxy->proxy_filename));
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
