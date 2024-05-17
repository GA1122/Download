GahpServer::Startup()
{
	char *gahp_path = NULL;
	ArgList gahp_args;
	int stdin_pipefds[2];
	int stdout_pipefds[2];
	int stderr_pipefds[2];
	int low_port;
	int high_port;
	Env newenv;
	char *tmp_char;

	if ( m_gahp_startup_failed ) {
		return false;
	} else if ( m_gahp_pid != -1 ) {
		return true;
	}

	if ( binary_path && strcmp( binary_path, GAHPCLIENT_DEFAULT_SERVER_PATH ) != 0 ) {
		gahp_path = strdup(binary_path);
		gahp_args.AppendArgsFromArgList(binary_args);
	} else {
		gahp_path = param("GAHP");

		char *args = param("GAHP_ARGS");
		MyString args_error;
		if(!gahp_args.AppendArgsV1RawOrV2Quoted(args,&args_error)) {
			EXCEPT("Failed to parse arguments: %s",args_error.Value());
		}
		free(args);
	}

	if (!gahp_path) return false;

	gahp_args.InsertArg( gahp_path, 0);

	newenv.SetEnv( "GAHP_TEMP", GridmanagerScratchDir );

	if ( get_port_range( FALSE, &low_port, &high_port ) == TRUE ) {
		std::string buff;
		sprintf( buff, "%d,%d", low_port, high_port );
		newenv.SetEnv( "GLOBUS_TCP_PORT_RANGE", buff.c_str() );
	}
	if ( get_port_range( TRUE, &low_port, &high_port ) == TRUE ) {
		std::string buff;
		sprintf( buff, "%d,%d", low_port, high_port );
		newenv.SetEnv( "GLOBUS_TCP_SOURCE_RANGE", buff.c_str() );
	}

	tmp_char = param("GLITE_LOCATION");
	if ( tmp_char ) {
		newenv.SetEnv( "GLITE_LOCATION", tmp_char );
		free( tmp_char );
	}

	tmp_char = param("SOAP_SSL_CA_FILE");
	if( tmp_char ) {
		newenv.SetEnv( "SOAP_SSL_CA_FILE", tmp_char );
		free( tmp_char );
	}

	tmp_char = param("SOAP_SSL_CA_DIR");
	if( tmp_char ) {
		newenv.SetEnv( "SOAP_SSL_CA_DIR", tmp_char );
		free( tmp_char );
	}

	if ( param_boolean( "SOAP_SSL_SKIP_HOST_CHECK", false ) ) {
		newenv.SetEnv( "SOAP_SSL_SKIP_HOST_CHECK", "True" );
	}

	if ( m_reaperid == -1 ) {
		m_reaperid = daemonCore->Register_Reaper(
				"GAHP Server",					
				(ReaperHandler)&GahpServer::Reaper,	 
				"GahpServer::Reaper",
				NULL
				);
	}


	bool is_c_gahp = false;
	if (strncmp(my_id, "CONDOR/", 7) == 0) {
		is_c_gahp = true;
	}

	if ( (daemonCore->Create_Pipe(stdin_pipefds, is_c_gahp) == FALSE) ||
	     (daemonCore->Create_Pipe(stdout_pipefds, true, false, true) == FALSE) ||
	     (daemonCore->Create_Pipe(stderr_pipefds, true, false, true) == FALSE)) 
	{
		dprintf(D_ALWAYS,"GahpServer::Startup - pipe() failed, errno=%d\n",
			errno);
		free( gahp_path );
		return false;
	}

	int io_redirect[3];
	io_redirect[0] = stdin_pipefds[0];	 
	io_redirect[1] = stdout_pipefds[1];  
	io_redirect[2] = stderr_pipefds[1];  

	m_gahp_pid = daemonCore->Create_Process(
			gahp_path,		 
			gahp_args,		 
			PRIV_USER_FINAL, 
			m_reaperid,		 
			FALSE,			 
			&newenv,	  	 
			NULL,			 
			NULL,			 
			NULL,			 
			io_redirect 	 
			);

	if ( m_gahp_pid == FALSE ) {
		dprintf(D_ALWAYS,"Failed to start GAHP server (%s)\n",
				gahp_path);
		free( gahp_path );
		m_gahp_pid = -1;
		return false;
	} else {
		dprintf(D_ALWAYS,"GAHP server pid = %d\n",m_gahp_pid);
	}

	free( gahp_path );

	daemonCore->Close_Pipe( io_redirect[0] );
	daemonCore->Close_Pipe( io_redirect[1] );
	daemonCore->Close_Pipe( io_redirect[2] );

	m_gahp_errorfd = stderr_pipefds[0];
	m_gahp_readfd = stdout_pipefds[0];
	m_gahp_writefd = stdin_pipefds[1];

	if ( command_version() == false ) {
		dprintf(D_ALWAYS,"Failed to read GAHP server version\n");
		m_gahp_startup_failed = true;
		return false;
	} else {
		dprintf(D_FULLDEBUG,"GAHP server version: %s\n",m_gahp_version);
	}

	if ( command_commands() == false ) {
		m_gahp_startup_failed = true;
		return false;
	}

	use_prefix = command_response_prefix( GAHP_PREFIX );

	if  ( !command_async_mode_on() ) {
		setPollInterval(m_pollInterval);
	} else {
		int result = daemonCore->Register_Pipe(m_gahp_readfd,
			"m_gahp_readfd",static_cast<PipeHandlercpp>(&GahpServer::pipe_ready),
			"&GahpServer::pipe_ready",this);
		if ( result == -1 ) {
			setPollInterval(m_pollInterval);
		} else {
		        setPollInterval(m_pollInterval * 12);
		}

		result = daemonCore->Register_Pipe(m_gahp_errorfd,
			  "m_gahp_errorfd",static_cast<PipeHandlercpp>(&GahpServer::err_pipe_ready),
			   "&GahpServer::err_pipe_ready",this);
	}
		
	if ( m_commands_supported->contains_anycase("CACHE_PROXY_FROM_FILE")==TRUE &&
		 m_commands_supported->contains_anycase("UNCACHE_PROXY")==TRUE &&
		 m_commands_supported->contains_anycase("USE_CACHED_PROXY")==TRUE ) {
		can_cache_proxies = true;
	}

	return true;
}
