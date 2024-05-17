GahpServer::command_response_prefix(const char *prefix)
{
	static const char* command = "RESPONSE_PREFIX";

	if  (m_commands_supported->contains_anycase(command)==FALSE) {
		return false;
	}

	std::string buf;
	int x = sprintf(buf,"%s %s",command,escapeGahpString(prefix));
	ASSERT( x > 0 );
	write_line(buf.c_str());

	Gahp_Args result;
	read_argv(result);
	if ( result.argc == 0 || result.argv[0][0] != 'S' ) {
		dprintf(D_ALWAYS,"GAHP command '%s' failed\n",command);
		return false;
	}

	return true;
}
