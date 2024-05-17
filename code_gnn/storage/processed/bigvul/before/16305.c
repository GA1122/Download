main( int argc, char *argv[] )
{
	char	*arg;
	char	**args = (char **)malloc(sizeof(char *)*(argc - 1));  
	int					nArgs = 0;				 
	int					i;
	char*	cmd_str;
	DCCollector* pool = NULL;
	char* scheddName = NULL;
	char* scheddAddr = NULL;
	param_functions *p_funcs = NULL;

	has_constraint = false;

	myDistro->Init( argc, argv );
	MyName = strrchr( argv[0], DIR_DELIM_CHAR );
	if( !MyName ) {
		MyName = argv[0];
	} else {
		MyName++;
	}

	cmd_str = strchr( MyName, '_');


	if (cmd_str && strncasecmp( cmd_str, "_hold", strlen("_hold") ) == MATCH) { 

		mode = JA_HOLD_JOBS;

	} else if ( cmd_str && 
			strncasecmp( cmd_str, "_release", strlen("_release") ) == MATCH ) {

		mode = JA_RELEASE_JOBS;

	} else if ( cmd_str && 
			strncasecmp( cmd_str, "_suspend", strlen("_suspend") ) == MATCH ) {

		mode = JA_SUSPEND_JOBS;

	} else if ( cmd_str && 
			strncasecmp( cmd_str, "_continue", strlen("_continue") ) == MATCH ) {

		mode = JA_CONTINUE_JOBS;

	}else if ( cmd_str && 
			strncasecmp( cmd_str, "_rm", strlen("_rm") ) == MATCH ) {

		mode = JA_REMOVE_JOBS;

	} else if( cmd_str && ! strncasecmp(cmd_str, "_vacate_job",
									strlen("_vacate_job")) ) {  

		mode = JA_VACATE_JOBS;

	} else {
		fprintf( stderr, "Unrecognized command name, \"%s\"\n", MyName ); 
		usage();
	}

	config();


	if( argc < 2 ) {
		fprintf( stderr, "You did not specify any jobs\n" ); 
		usage();
	}

#if !defined(WIN32)
	install_sig_handler(SIGPIPE, SIG_IGN );
#endif

	for( argv++; (arg = *argv); argv++ ) {
		if( arg[0] == '-' ) {
            if (match_prefix(arg, "-debug")) {
				Termlog = 1;
				p_funcs = get_param_functions();
				dprintf_config ("TOOL", p_funcs);
            } else if (match_prefix(arg, "-constraint")) {
				args[nArgs] = arg;
				nArgs++;
				argv++;
				if( ! *argv ) {
					fprintf( stderr, 
							 "%s: -constraint requires another argument\n", 
							 MyName);
					exit(1);
				}				
				args[nArgs] = *argv;
				nArgs++;
				ConstraintArg = true;
            } else if (match_prefix(arg, "-all")) {
                All = true;
            } else if (match_prefix(arg, "-addr")) {
                argv++;
                if( ! *argv ) {
                    fprintf( stderr, 
                             "%s: -addr requires another argument\n", 
                             MyName);
                    exit(1);
                }				
                if( is_valid_sinful(*argv) ) {
                    scheddAddr = strdup(*argv);
                    if( ! scheddAddr ) {
                        fprintf( stderr, "Out of memory!\n" );
                        exit(1);
                    }
                } else {
                    fprintf( stderr, 
                             "%s: \"%s\" is not a valid address\n",
                             MyName, *argv );
                    fprintf( stderr, "Should be of the form "
                             "<ip.address.here:port>\n" );
                    fprintf( stderr, 
                             "For example: <123.456.789.123:6789>\n" );
                    exit( 1 );
                }
			} else if (match_prefix(arg, "-reason")) {
				argv++;
				if( ! *argv ) {
					fprintf( stderr, 
							 "%s: -reason requires another argument\n", 
							 MyName);
					exit(1);
				}		
				actionReason = strdup(*argv);		
				if( ! actionReason ) {
					fprintf( stderr, "Out of memory!\n" );
					exit(1);
				}
			} else if (match_prefix(arg, "-subcode")) {
				argv++;
				if( ! *argv ) {
					fprintf( stderr, 
							 "%s: -subcode requires another argument\n", 
							 MyName);
					exit(1);
				}		
				char *end = NULL;
				  strtol(*argv,&end,10);
				if( !end || *end || end==*argv ) {
					fprintf( stderr, "Invalid -subcode %s!\n", *argv );
					exit(1);
				}
				holdReasonSubCode = strdup(*argv);
				ASSERT( holdReasonSubCode );
            } else if (match_prefix(arg, "-forcex")) {
				if( mode == JA_REMOVE_JOBS ) {
					mode = JA_REMOVE_X_JOBS;
				} else {
                    fprintf( stderr, 
                             "-forcex is only valid with condor_rm\n" );
					usage();
				}
            } else if (match_prefix(arg, "-fast")) {
				if( mode == JA_VACATE_JOBS ) {
					mode = JA_VACATE_FAST_JOBS;
				} else {
                    fprintf( stderr, 
                             "-fast is only valid with condor_vacate_job\n" );
					usage();
				}
            } else if (match_prefix(arg, "-name")) {
				argv++;
				if( ! *argv ) {
					fprintf( stderr, "%s: -name requires another argument\n", 
							 MyName);
					exit(1);
				}				
				if( !(scheddName = get_daemon_name(*argv)) ) { 
					fprintf( stderr, "%s: unknown host %s\n", 
							 MyName, get_host_part(*argv) );
					exit(1);
				}
            } else if (match_prefix(arg, "-pool")) {
				argv++;
				if( ! *argv ) {
					fprintf( stderr, "%s: -pool requires another argument\n", 
							 MyName);
					exit(1);
				}				
				if( pool ) {
					delete pool;
				}
				pool = new DCCollector( *argv );
				if( ! pool->addr() ) {
					fprintf( stderr, "%s: %s\n", MyName, pool->error() );
					exit(1);
				}
            } else if (match_prefix(arg, "-version")) {
				version();
            } else if (match_prefix(arg, "-help")) {
				usage(0);
            } else {
				fprintf( stderr, "Unrecognized option: %s\n", arg ); 
				usage();
			}
		} else {
			if( All ) {
				usage();
			}
			args[nArgs] = arg;
			nArgs++;
			UserJobIdArg = true;
		}
	}

	if( ! (All || nArgs) ) {
		fprintf( stderr, "You did not specify any jobs\n" ); 
		usage();
	}

	if ( ConstraintArg && UserJobIdArg ) {
		fprintf( stderr, "You can't use both -constraint and usernames or job ids\n" );
		usage();
	}

	if( actionReason == NULL ) {
		switch( mode ) {
		case JA_RELEASE_JOBS:
			actionReason = strdup("via condor_release");
			break;
		case JA_REMOVE_X_JOBS:
			actionReason = strdup("via condor_rm -forcex");
			break;
		case JA_REMOVE_JOBS:
			actionReason = strdup("via condor_rm");
			break;
		case JA_HOLD_JOBS:
			actionReason = strdup("via condor_hold");
			break;
		case JA_SUSPEND_JOBS:
			actionReason = strdup("via condor_suspend");
			break;
		case JA_CONTINUE_JOBS:
			actionReason = strdup("via condor_continue");
			break;
		default:
			actionReason = NULL;
		}
	}

	if( ! scheddAddr ) {
		schedd = new DCSchedd( scheddName, pool ? pool->addr() : NULL );
	} else {
		schedd = new DCSchedd( scheddAddr );
	}
	if( ! schedd->locate() ) {
		fprintf( stderr, "%s: %s\n", MyName, schedd->error() ); 
		exit( 1 );
	}

	if(mode == JA_REMOVE_X_JOBS) {
		if( mayUserForceRm() == false) {
			fprintf( stderr, "Remove aborted. condor_rm -forcex has been disabled by the administrator.\n" );
			exit( 1 );
		}
	}

	if( All ) {
		handleAll();
	} else {
		for(i = 0; i < nArgs; i++) {
			if( match_prefix( args[i], "-constraint" ) ) {
				i++;
				addConstraint( args[i] );
			} else {
				procArg(args[i]);
			}
		}
	}

	handleConstraints();

	if( job_ids ) {
		CondorError errstack;
		ClassAd* result_ad = doWorkByList( job_ids, &errstack );
		if (had_error) {
			fprintf( stderr, "%s\n", errstack.getFullText(true) );
		}
		printNewMessages( result_ad, job_ids );
		delete( result_ad );
	}

	if ( mode == JA_RELEASE_JOBS && had_error == false ) {
		Daemon  my_schedd(DT_SCHEDD, NULL, NULL);
		CondorError errstack;
		if (!my_schedd.sendCommand(RESCHEDULE, Stream::safe_sock, 0, &errstack)) {
			fprintf( stderr, "%s\n", errstack.getFullText(true) );
		}
	}

	return had_error;
}
