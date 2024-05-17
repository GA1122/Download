make_log_entry( httpd_conn* hc, struct timeval* nowP )
    {
    char* ru;
    char url[305];
    char bytes[40];

    if ( hc->hs->no_log )
	return;

     

     
    if ( hc->remoteuser[0] != '\0' )
	ru = hc->remoteuser;
    else
	ru = "-";
     
    if ( hc->hs->vhost && ! hc->tildemapped )
	(void) my_snprintf( url, sizeof(url),
	    "/%.100s%.200s",
	    hc->hostname == (char*) 0 ? hc->hs->server_hostname : hc->hostname,
	    hc->encodedurl );
    else
	(void) my_snprintf( url, sizeof(url),
	    "%.200s", hc->encodedurl );
     
    if ( hc->bytes_sent >= 0 )
	(void) my_snprintf(
	    bytes, sizeof(bytes), "%lld", (int64_t) hc->bytes_sent );
    else
	(void) strcpy( bytes, "-" );

     
    if ( hc->hs->logfp != (FILE*) 0 )
	{
	time_t now;
	struct tm* t;
	const char* cernfmt_nozone = "%d/%b/%Y:%H:%M:%S";
	char date_nozone[100];
	int zone;
	char sign;
	char date[100];

	 
	if ( nowP != (struct timeval*) 0 )
	    now = nowP->tv_sec;
	else
	    now = time( (time_t*) 0 );
	 
	t = localtime( &now );
	(void) strftime( date_nozone, sizeof(date_nozone), cernfmt_nozone, t );
	zone = t->tm_gmtoff / 60L;
	if ( zone >= 0 )
	    sign = '+';
	else
	    {
	    sign = '-';
	    zone = -zone;
	    }
	zone = ( zone / 60 ) * 100 + zone % 60;
	(void) my_snprintf( date, sizeof(date),
	    "%s %c%04d", date_nozone, sign, zone );
	 
	(void) fprintf( hc->hs->logfp,
	    "%.80s - %.80s [%s] \"%.80s %.300s %.80s\" %d %s \"%.200s\" \"%.200s\"\n",
	    httpd_ntoa( &hc->client_addr ), ru, date,
	    httpd_method_str( hc->method ), url, hc->protocol,
	    hc->status, bytes, hc->referer, hc->useragent );
#ifdef FLUSH_LOG_EVERY_TIME
	(void) fflush( hc->hs->logfp );
#endif
	}
    else
	syslog( LOG_INFO,
	    "%.80s - %.80s \"%.80s %.200s %.80s\" %d %s \"%.200s\" \"%.200s\"",
	    httpd_ntoa( &hc->client_addr ), ru,
	    httpd_method_str( hc->method ), url, hc->protocol,
	    hc->status, bytes, hc->referer, hc->useragent );
    }
