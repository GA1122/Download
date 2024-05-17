GahpServer::write_line(const char *command, int req, const char *args)
{
	if ( !command || m_gahp_writefd == -1 ) {
		return;
	}

	char buf[20];
	sprintf(buf," %d%s",req,args?" ":"");
	daemonCore->Write_Pipe(m_gahp_writefd,command,strlen(command));
	daemonCore->Write_Pipe(m_gahp_writefd,buf,strlen(buf));
	if ( args ) {
		daemonCore->Write_Pipe(m_gahp_writefd,args,strlen(args));
	}
	daemonCore->Write_Pipe(m_gahp_writefd,"\r\n",2);

	if ( logGahpIo ) {
		std::string debug = command;
		if ( args ) {
			sprintf( debug, "'%s%s%s'", command, buf, args );
		} else {
			sprintf( debug, "'%s%s'", command, buf );
		}
		if ( logGahpIoSize > 0 && debug.length() > logGahpIoSize ) {
			debug.erase( logGahpIoSize, std::string::npos );
			debug += "...";
		}
		dprintf( D_FULLDEBUG, "GAHP[%d] <- %s\n", m_gahp_pid,
				 debug.c_str() );
	}

	return;
}
