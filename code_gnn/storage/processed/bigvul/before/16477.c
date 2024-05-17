main( int argc, char *argv[] )
{
#ifndef WIN32
    install_sig_handler(SIGPIPE, SIG_IGN );
#endif

	config(false,false,false);
	
	MyName = argv[0];
	myDistro->Init( argc, argv );
	config();
	init_params();
	BadFiles = new StringList;
	param_functions *p_funcs = NULL;

	for( argv++; *argv; argv++ ) {
		if( (*argv)[0] == '-' ) {
			switch( (*argv)[1] ) {
			
			  case 'd':
                Termlog = 1;
			  case 'v':
				VerboseFlag = TRUE;
				break;

			  case 'm':
				MailFlag = TRUE;
				break;

			  case 'r':
				RmFlag = TRUE;
				break;

			  default:
				usage();

			}
		} else {
			usage();
		}
	}
	
	p_funcs = get_param_functions();
	dprintf_config("TOOL", p_funcs);
	if (VerboseFlag)
	{
		std::string szVerbose="D_FULLDEBUG";
		char * pval = param("TOOL_DEBUG");
		if( pval ) {
			szVerbose+="|";
			szVerbose+=pval;
			free( pval );
		}
		_condor_set_debug_flags( szVerbose.c_str() );
		
	}
	dprintf( D_ALWAYS, "********************************\n");
	dprintf( D_ALWAYS, "STARTING: condor_preen\n");
	dprintf( D_ALWAYS, "********************************\n");
	
	check_spool_dir();
	check_execute_dir();
	check_log_dir();
	check_daemon_sock_dir();
	check_tmp_dir();

	if( !BadFiles->isEmpty() ) {
		produce_output();
	}

	delete BadFiles;

	dprintf( D_ALWAYS, "********************************\n");
	dprintf( D_ALWAYS, "ENDING: condor_preen\n");
	dprintf( D_ALWAYS, "********************************\n");
	
	return 0;
}
