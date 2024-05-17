auth_check2( httpd_conn* hc, char* dirname  )
    {
    static char* authpath;
    static size_t maxauthpath = 0;
    struct stat sb;
    char authinfo[500];
    char* authpass;
    char* colon;
    int l;
    FILE* fp;
    char line[500];
    char* cryp;
    static char* prevauthpath;
    static size_t maxprevauthpath = 0;
    static time_t prevmtime;
    static char* prevuser;
    static size_t maxprevuser = 0;
    static char* prevcryp;
    static size_t maxprevcryp = 0;
    char *crypt_result;

     
    httpd_realloc_str(
	&authpath, &maxauthpath, strlen( dirname ) + 1 + sizeof(AUTH_FILE) );
    (void) my_snprintf( authpath, maxauthpath, "%s/%s", dirname, AUTH_FILE );

     
    if ( stat( authpath, &sb ) < 0 )
	 
	return 0;

     
    if ( hc->authorization[0] == '\0' ||
	 strncmp( hc->authorization, "Basic ", 6 ) != 0 )
	{
	 
	send_authenticate( hc, dirname );
	return -1;
	}

     
    l = b64_decode(
	&(hc->authorization[6]), (unsigned char*) authinfo,
	sizeof(authinfo) - 1 );
    authinfo[l] = '\0';
     
    authpass = strchr( authinfo, ':' );
    if ( authpass == (char*) 0 )
	{
	 
	send_authenticate( hc, dirname );
	return -1;
	}
    *authpass++ = '\0';
     
    colon = strchr( authpass, ':' );
    if ( colon != (char*) 0 )
	*colon = '\0';

     
    if ( maxprevauthpath != 0 &&
	 strcmp( authpath, prevauthpath ) == 0 &&
	 sb.st_mtime == prevmtime &&
	 strcmp( authinfo, prevuser ) == 0 )
	{
	 
	crypt_result = crypt( authpass, prevcryp );
	if ( ! crypt_result )
	    return -1;
	if ( strcmp( crypt_result, prevcryp ) == 0 )
	    {
	     
	    httpd_realloc_str(
		&hc->remoteuser, &hc->maxremoteuser, strlen( authinfo ) );
	    (void) strcpy( hc->remoteuser, authinfo );
	    return 1;
	    }
	else
	    {
	     
	    send_authenticate( hc, dirname );
	    return -1;
	    }
	}

     
    fp = fopen( authpath, "r" );
    if ( fp == (FILE*) 0 )
	{
	 
	syslog(
	    LOG_ERR, "%.80s auth file %.80s could not be opened - %m",
	    httpd_ntoa( &hc->client_addr ), authpath );
	httpd_send_err(
	    hc, 403, err403title, "",
	    ERROR_FORM( err403form, "The requested URL '%.80s' is protected by an authentication file, but the authentication file cannot be opened.\n" ),
	    hc->encodedurl );
	return -1;
	}

     
    while ( fgets( line, sizeof(line), fp ) != (char*) 0 )
	{
	 
	l = strlen( line );
	if ( line[l - 1] == '\n' )
	    line[l - 1] = '\0';
	 
	cryp = strchr( line, ':' );
	if ( cryp == (char*) 0 )
	    continue;
	*cryp++ = '\0';
	 
	if ( strcmp( line, authinfo ) == 0 )
	    {
	     
	    (void) fclose( fp );
	     
	    crypt_result = crypt( authpass, cryp );
	    if ( ! crypt_result )
		return -1;
	    if ( strcmp( crypt_result, cryp ) == 0 )
		{
		 
		httpd_realloc_str(
		    &hc->remoteuser, &hc->maxremoteuser, strlen( line ) );
		(void) strcpy( hc->remoteuser, line );
		 
		httpd_realloc_str(
		    &prevauthpath, &maxprevauthpath, strlen( authpath ) );
		(void) strcpy( prevauthpath, authpath );
		prevmtime = sb.st_mtime;
		httpd_realloc_str(
		    &prevuser, &maxprevuser, strlen( authinfo ) );
		(void) strcpy( prevuser, authinfo );
		httpd_realloc_str( &prevcryp, &maxprevcryp, strlen( cryp ) );
		(void) strcpy( prevcryp, cryp );
		return 1;
		}
	    else
		{
		 
		send_authenticate( hc, dirname );
		return -1;
		}
	    }
	}

     
    (void) fclose( fp );
    send_authenticate( hc, dirname );
    return -1;
    }
