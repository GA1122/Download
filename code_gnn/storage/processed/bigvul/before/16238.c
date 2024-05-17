GahpServer::write_line(const char *command)
{
	if ( !command || m_gahp_writefd == -1 ) {
		return;
	}
	
	daemonCore->Write_Pipe(m_gahp_writefd,command,strlen(command));
	daemonCore->Write_Pipe(m_gahp_writefd,"\r\n",2);

	if ( logGahpIo ) {
		std::string debug = command;
		sprintf( debug, "'%s'", command );
		if ( logGahpIoSize > 0 && debug.length() > logGahpIoSize ) {
			debug.erase( logGahpIoSize, std::string::npos );
			debug += "...";
		}
		dprintf( D_FULLDEBUG, "GAHP[%d] <- %s\n", m_gahp_pid,
				 debug.c_str() );
	}

	return;
}
