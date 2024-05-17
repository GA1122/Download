GahpServer::command_commands()
{
	write_line("COMMANDS");
	Gahp_Args result;
	read_argv(result);
	if ( result.argc == 0 || result.argv[0][0] != 'S' ) {
		dprintf(D_ALWAYS,"GAHP command 'COMMANDS' failed\n");
		return false;
	}

	if ( m_commands_supported ) {
		delete m_commands_supported;
		m_commands_supported = NULL;
	}
	m_commands_supported = new StringList();
	ASSERT(m_commands_supported);
	for ( int i = 1; i < result.argc; i++ ) {
		m_commands_supported->append(result.argv[i]);
	}

	return true;
}
