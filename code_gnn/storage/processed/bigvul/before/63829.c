make_envp( httpd_conn* hc )
    {
    static char* envp[50];
    int envn;
    char* cp;
    char buf[256];

    envn = 0;
    envp[envn++] = build_env( "PATH=%s", CGI_PATH );
#ifdef CGI_LD_LIBRARY_PATH
    envp[envn++] = build_env( "LD_LIBRARY_PATH=%s", CGI_LD_LIBRARY_PATH );
#endif  
    envp[envn++] = build_env( "SERVER_SOFTWARE=%s", SERVER_SOFTWARE );
     
    if ( hc->hs->vhost && hc->hostname != (char*) 0 )
	cp = hc->hostname;
    else
	cp = hc->hs->server_hostname;
    if ( cp != (char*) 0 )
	envp[envn++] = build_env( "SERVER_NAME=%s", cp );
    envp[envn++] = "GATEWAY_INTERFACE=CGI/1.1";
    envp[envn++] = build_env("SERVER_PROTOCOL=%s", hc->protocol);
    (void) my_snprintf( buf, sizeof(buf), "%d", (int) hc->hs->port );
    envp[envn++] = build_env( "SERVER_PORT=%s", buf );
    envp[envn++] = build_env(
	"REQUEST_METHOD=%s", httpd_method_str( hc->method ) );
    if ( hc->pathinfo[0] != '\0' )
	{
	char* cp2;
	size_t l;
	envp[envn++] = build_env( "PATH_INFO=/%s", hc->pathinfo );
	l = strlen( hc->hs->cwd ) + strlen( hc->pathinfo ) + 1;
	cp2 = NEW( char, l );
	if ( cp2 != (char*) 0 )
	    {
	    (void) my_snprintf( cp2, l, "%s%s", hc->hs->cwd, hc->pathinfo );
	    envp[envn++] = build_env( "PATH_TRANSLATED=%s", cp2 );
	    }
	}
    envp[envn++] = build_env(
	"SCRIPT_NAME=/%s", strcmp( hc->origfilename, "." ) == 0 ?
	"" : hc->origfilename );
    if ( hc->query[0] != '\0')
	envp[envn++] = build_env( "QUERY_STRING=%s", hc->query );
    envp[envn++] = build_env(
	"REMOTE_ADDR=%s", httpd_ntoa( &hc->client_addr ) );
    if ( hc->referer[0] != '\0' )
	envp[envn++] = build_env( "HTTP_REFERER=%s", hc->referer );
    if ( hc->useragent[0] != '\0' )
	envp[envn++] = build_env( "HTTP_USER_AGENT=%s", hc->useragent );
    if ( hc->accept[0] != '\0' )
	envp[envn++] = build_env( "HTTP_ACCEPT=%s", hc->accept );
    if ( hc->accepte[0] != '\0' )
	envp[envn++] = build_env( "HTTP_ACCEPT_ENCODING=%s", hc->accepte );
    if ( hc->acceptl[0] != '\0' )
	envp[envn++] = build_env( "HTTP_ACCEPT_LANGUAGE=%s", hc->acceptl );
    if ( hc->cookie[0] != '\0' )
	envp[envn++] = build_env( "HTTP_COOKIE=%s", hc->cookie );
    if ( hc->contenttype[0] != '\0' )
	envp[envn++] = build_env( "CONTENT_TYPE=%s", hc->contenttype );
    if ( hc->hdrhost[0] != '\0' )
	envp[envn++] = build_env( "HTTP_HOST=%s", hc->hdrhost );
    if ( hc->contentlength != -1 )
	{
	(void) my_snprintf(
	    buf, sizeof(buf), "%lu", (unsigned long) hc->contentlength );
	envp[envn++] = build_env( "CONTENT_LENGTH=%s", buf );
	}
    if ( hc->remoteuser[0] != '\0' )
	envp[envn++] = build_env( "REMOTE_USER=%s", hc->remoteuser );
    if ( hc->authorization[0] != '\0' )
	envp[envn++] = build_env( "AUTH_TYPE=%s", "Basic" );
	 
    if ( getenv( "TZ" ) != (char*) 0 )
	envp[envn++] = build_env( "TZ=%s", getenv( "TZ" ) );
    envp[envn++] = build_env( "CGI_PATTERN=%s", hc->hs->cgi_pattern );

    envp[envn] = (char*) 0;
    return envp;
    }
