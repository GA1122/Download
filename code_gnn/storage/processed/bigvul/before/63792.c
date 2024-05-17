cgi_child( httpd_conn* hc )
    {
    int r;
    char** argp;
    char** envp;
    char* binary;
    char* directory;

     
    (void) fcntl( hc->conn_fd, F_SETFD, 0 );

     
    closelog();

     
    if ( hc->conn_fd == STDIN_FILENO || hc->conn_fd == STDOUT_FILENO || hc->conn_fd == STDERR_FILENO )
	{
	int newfd = dup2( hc->conn_fd, STDERR_FILENO + 1 );
	if ( newfd >= 0 )
	    hc->conn_fd = newfd;
	 
	}

     
    envp = make_envp( hc );

     
    argp = make_argp( hc );

     
    if ( hc->method == METHOD_POST && hc->read_idx > hc->checked_idx )
	{
	int p[2];

	if ( pipe( p ) < 0 )
	    {
	    syslog( LOG_ERR, "pipe - %m" );
	    httpd_send_err( hc, 500, err500title, "", err500form, hc->encodedurl );
	    httpd_write_response( hc );
	    exit( 1 );
	    }
	r = fork( );
	if ( r < 0 )
	    {
	    syslog( LOG_ERR, "fork - %m" );
	    httpd_send_err( hc, 500, err500title, "", err500form, hc->encodedurl );
	    httpd_write_response( hc );
	    exit( 1 );
	    }
	if ( r == 0 )
	    {
	     
	    sub_process = 1;
	    (void) close( p[0] );
	    cgi_interpose_input( hc, p[1] );
	    exit( 0 );
	    }
	 
	(void) close( p[1] );
	if ( p[0] != STDIN_FILENO )
	    {
	    (void) dup2( p[0], STDIN_FILENO );
	    (void) close( p[0] );
	    }
	}
    else
	{
	 
	if ( hc->conn_fd != STDIN_FILENO )
	    (void) dup2( hc->conn_fd, STDIN_FILENO );
	}

     
    if ( strncmp( argp[0], "nph-", 4 ) != 0 && hc->mime_flag )
	{
	int p[2];

	if ( pipe( p ) < 0 )
	    {
	    syslog( LOG_ERR, "pipe - %m" );
	    httpd_send_err( hc, 500, err500title, "", err500form, hc->encodedurl );
	    httpd_write_response( hc );
	    exit( 1 );
	    }
	r = fork( );
	if ( r < 0 )
	    {
	    syslog( LOG_ERR, "fork - %m" );
	    httpd_send_err( hc, 500, err500title, "", err500form, hc->encodedurl );
	    httpd_write_response( hc );
	    exit( 1 );
	    }
	if ( r == 0 )
	    {
	     
	    sub_process = 1;
	    (void) close( p[1] );
	    cgi_interpose_output( hc, p[0] );
	    exit( 0 );
	    }
	 
	(void) close( p[0] );
	if ( p[1] != STDOUT_FILENO )
	    (void) dup2( p[1], STDOUT_FILENO );
	if ( p[1] != STDERR_FILENO )
	    (void) dup2( p[1], STDERR_FILENO );
	if ( p[1] != STDOUT_FILENO && p[1] != STDERR_FILENO )
	    (void) close( p[1] );
	}
    else
	{
	 
	if ( hc->conn_fd != STDOUT_FILENO )
	    (void) dup2( hc->conn_fd, STDOUT_FILENO );
	if ( hc->conn_fd != STDERR_FILENO )
	    (void) dup2( hc->conn_fd, STDERR_FILENO );
	}

     
     

#ifdef CGI_NICE
     
    (void) nice( CGI_NICE );
#endif  

     
    directory = strdup( hc->expnfilename );
    if ( directory == (char*) 0 )
	binary = hc->expnfilename;       
    else
	{
	binary = strrchr( directory, '/' );
	if ( binary == (char*) 0 )
	    binary = hc->expnfilename;
	else
	    {
	    *binary++ = '\0';
	    (void) chdir( directory );   
	    }
	}

     
#ifdef HAVE_SIGSET
    (void) sigset( SIGPIPE, SIG_DFL );
#else  
    (void) signal( SIGPIPE, SIG_DFL );
#endif  

     
    (void) execve( binary, argp, envp );

     
    syslog( LOG_ERR, "execve %.80s - %m", hc->expnfilename );
    httpd_send_err( hc, 500, err500title, "", err500form, hc->encodedurl );
    httpd_write_response( hc );
    exit( 1 );
    }
