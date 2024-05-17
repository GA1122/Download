GahpServer::read_argv(Gahp_Args &g_args)
{
	static char* buf = NULL;
	int ibuf = 0;
	int result = 0;
	bool trash_this_line = false;
	bool escape_seen = false;
	static const int buf_size = 1024 * 500;

	g_args.reset();

	if ( m_gahp_readfd == -1 ) {
		if ( logGahpIo ) {
			dprintf( D_FULLDEBUG, "GAHP[%d] -> (no pipe)\n", m_gahp_pid );
		}
		return;
	}

	if ( buf == NULL ) {
		buf = (char*)malloc(buf_size);
	}

	ibuf = 0;

	for (;;) {

		ASSERT(ibuf < buf_size);
		result = buffered_read(m_gahp_readfd, &(buf[ibuf]), 1 );

		 
		if ( result < 0 ) {		 
			continue;
		}
		if ( result == 0 ) {	 
			g_args.reset();
			if ( logGahpIo ) {
				dprintf( D_FULLDEBUG, "GAHP[%d] -> EOF\n", m_gahp_pid );
			}
			return;
		}

		 
		if ( escape_seen ) {
			ibuf++;
			escape_seen = false;
			continue;
		}

		 
		if ( buf[ibuf] == '\\' ) {
			escape_seen = true;
			continue;
		}

		 
		if ( buf[ibuf] == '\r' ) {
			continue;
		}

		 
		if ( buf[ibuf] == ' ' ) {
			buf[ibuf] = '\0';
			g_args.add_arg( strdup( buf ) );
			ibuf = 0;
			continue;
		}

		 
		if ( buf[ibuf]=='\n' ) { 
			buf[ibuf] = 0;
			g_args.add_arg( strdup( buf ) );

			trash_this_line = false;
			if ( use_prefix ) {
				if ( g_args.argc > 0 && 
					 strncmp(GAHP_PREFIX,g_args.argv[0],GAHP_PREFIX_LEN)==0)
				{
					memmove(g_args.argv[0],&(g_args.argv[0][GAHP_PREFIX_LEN]),
							1 + strlen(&(g_args.argv[0][GAHP_PREFIX_LEN])));
				} else {
					trash_this_line = true;
				}
			}

			if ( logGahpIo ) {
				static std::string debug;
				debug = "";
				if( g_args.argc > 0 ) {
					debug += "'";
					for ( int i = 0; i < g_args.argc; i++ ) {
						if ( i != 0 ) {
							debug += "' '";
						}
						if ( g_args.argv[i] ) {
							debug += g_args.argv[i];
						}
						if ( logGahpIoSize > 0 &&
							 debug.length() > logGahpIoSize ) {
							break;
						}
					}
					debug += "'";
				}
				if ( logGahpIoSize > 0 && debug.length() > logGahpIoSize ) {
					debug.erase( logGahpIoSize, std::string::npos );
					debug += "...";
				}
				dprintf( D_FULLDEBUG, "GAHP[%d] %s-> %s\n", m_gahp_pid,
						 trash_this_line ? "(unprefixed) " : "",
						 debug.c_str() );
			}

			if ( trash_this_line==false && g_args.argc == 1 &&
				 g_args.argv[0][0] == 'R' ) {
				if ( skip_next_r ) {
					skip_next_r = false;
				} else {
					poll_real_soon();
				}
				trash_this_line = true;
			}

			if ( trash_this_line ) {
				g_args.reset();
				ibuf = 0;
				continue;	 
			}

			if ( !m_in_results && buffered_peek() > 0 ) {
				skip_next_r = true;
				poll_real_soon();
			}

			return;
		}

		 
		ibuf++;

	}	 
}
