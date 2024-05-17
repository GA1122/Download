expand_symlinks( char* path, char** restP, int no_symlink_check, int tildemapped )
    {
    static char* checked;
    static char* rest;
    char link[5000];
    static size_t maxchecked = 0, maxrest = 0;
    size_t checkedlen, restlen, linklen, prevcheckedlen, prevrestlen;
    int nlinks, i;
    char* r;
    char* cp1;
    char* cp2;

    if ( no_symlink_check )
	{
	 
	struct stat sb;
	if ( stat( path, &sb ) != -1 )
	    {
	    checkedlen = strlen( path );
	    httpd_realloc_str( &checked, &maxchecked, checkedlen );
	    (void) strcpy( checked, path );
	     
	    while ( checkedlen && checked[checkedlen - 1] == '/' )
		{
		checked[checkedlen - 1] = '\0';
		--checkedlen;
		}
	    httpd_realloc_str( &rest, &maxrest, 0 );
	    rest[0] = '\0';
	    *restP = rest;
	    return checked;
	    }
	}

     
    httpd_realloc_str( &checked, &maxchecked, 1 );
    checked[0] = '\0';
    checkedlen = 0;
    restlen = strlen( path );
    httpd_realloc_str( &rest, &maxrest, restlen );
    (void) strcpy( rest, path );
    if ( restlen && rest[restlen - 1] == '/' )
	rest[--restlen] = '\0';          
    if ( ! tildemapped )
	 
	while ( rest[0] == '/' )
	    {
	     
	    (void) memmove( rest, &(rest[1]), strlen(rest) );
	    --restlen;
	    }
    r = rest;
    nlinks = 0;

     
    while ( restlen > 0 )
	{
	 
	prevcheckedlen = checkedlen;
	prevrestlen = restlen;

	 
	cp1 = strchr( r, '/' );
	if ( cp1 != (char*) 0 )
	    {
	    i = cp1 - r;
	    if ( i == 0 )
		{
		 
		httpd_realloc_str( &checked, &maxchecked, checkedlen + 1 );
		(void) strncpy( &checked[checkedlen], r, 1 );
		checkedlen += 1;
		}
	    else if ( strncmp( r, "..", MAX( i, 2 ) ) == 0 )
		{
		 
		if ( checkedlen != 0 )
		    {
		    cp2 = strrchr( checked, '/' );
		    if ( cp2 == (char*) 0 )
			checkedlen = 0;
		    else if ( cp2 == checked )
			checkedlen = 1;
		    else
			checkedlen = cp2 - checked;
		    }
		}
	    else
		{
		httpd_realloc_str( &checked, &maxchecked, checkedlen + 1 + i );
		if ( checkedlen > 0 && checked[checkedlen-1] != '/' )
		    checked[checkedlen++] = '/';
		(void) strncpy( &checked[checkedlen], r, i );
		checkedlen += i;
		}
	    checked[checkedlen] = '\0';
	    r += i + 1;
	    restlen -= i + 1;
	    }
	else
	    {
	     
	    if ( strcmp( r, ".." ) == 0 )
		{
		 
		if ( checkedlen != 0 )
		    {
		    cp2 = strrchr( checked, '/' );
		    if ( cp2 == (char*) 0 )
			checkedlen = 0;
		    else if ( cp2 == checked )
			checkedlen = 1;
		    else
			checkedlen = cp2 - checked;
		    checked[checkedlen] = '\0';
		    }
		}
	    else
		{
		httpd_realloc_str(
		    &checked, &maxchecked, checkedlen + 1 + restlen );
		if ( checkedlen > 0 && checked[checkedlen-1] != '/' )
		    checked[checkedlen++] = '/';
		(void) strcpy( &checked[checkedlen], r );
		checkedlen += restlen;
		}
	    r += restlen;
	    restlen = 0;
	    }

	 
	if ( checked[0] == '\0' )
	    continue;
	linklen = readlink( checked, link, sizeof(link) - 1 );
	if ( linklen == -1 )
	    {
	    if ( errno == EINVAL )
		continue;                
	    if ( errno == EACCES || errno == ENOENT || errno == ENOTDIR )
		{
		 
		*restP = r - ( prevrestlen - restlen );
		if ( prevcheckedlen == 0 )
		    (void) strcpy( checked, "." );
		else
		    checked[prevcheckedlen] = '\0';
		return checked;
		}
	    syslog( LOG_ERR, "readlink %.80s - %m", checked );
	    return (char*) 0;
	    }
	++nlinks;
	if ( nlinks > MAX_LINKS )
	    {
	    syslog( LOG_ERR, "too many symlinks in %.80s", path );
	    return (char*) 0;
	    }
	link[linklen] = '\0';
	if ( link[linklen - 1] == '/' )
	    link[--linklen] = '\0';      

	 
	if ( restlen != 0 )
	    {
	    (void) strcpy( rest, r );
	    httpd_realloc_str( &rest, &maxrest, restlen + linklen + 1 );
	    for ( i = restlen; i >= 0; --i )
		rest[i + linklen + 1] = rest[i];
	    (void) strcpy( rest, link );
	    rest[linklen] = '/';
	    restlen += linklen + 1;
	    r = rest;
	    }
	else
	    {
	     
	    httpd_realloc_str( &rest, &maxrest, linklen );
	    (void) strcpy( rest, link );
	    restlen = linklen;
	    r = rest;
	    }

	if ( rest[0] == '/' )
	    {
	     
	    checked[0] = '\0';
	    checkedlen = 0;
	    }
	else
	    {
	     
	    checkedlen = prevcheckedlen;
	    checked[checkedlen] = '\0';
	    }
	}

     
    *restP = r;
    if ( checked[0] == '\0' )
	(void) strcpy( checked, "." );
    return checked;
    }
