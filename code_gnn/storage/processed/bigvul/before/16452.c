sysapi_translate_opsys( const char *sysname,
						const char *release,
						const char *version,
                        int         append_version)
{
	char tmp[64];
    char ver[24];
    const char * pver="";
	char *tmpopsys;

	if( !strcmp(sysname, "Linux") ) {
		sprintf( tmp, "LINUX" );
	}
	else if( !strcmp(sysname, "linux") ) {  
		sprintf( tmp, "LINUX" );
	}

	else if( !strcmp(sysname, "SunOS")
		|| !strcmp(sysname, "solaris" ) )  
	{
        sprintf( tmp, "SOLARIS" );
		if ( !strcmp(release, "2.10")  
			|| !strcmp(release, "5.10") )
		{
			pver = "210";
		}
		else if ( !strcmp(release, "2.9")  
			|| !strcmp(release, "5.9") )
		{
			pver = "29";
		}
		else if ( !strcmp(release, "2.8")  
			|| !strcmp(release, "5.8") )
		{
			pver = "28";
		}
		else if ( !strcmp(release, "2.7")  
			|| !strcmp(release, "5.7") )
		{
			pver = "27";
		}
		else if( !strcmp(release, "5.6")
			||  !strcmp(release, "2.6") )  
		{
			pver = "26";
		}
		else if ( !strcmp(release, "5.5.1")
			|| !strcmp(release, "2.5.1") )  
		{
			pver = "251";
		}
		else if ( !strcmp(release, "5.5")
			|| !strcmp(release, "2.5") )  
		{
			pver = "25";
		}
		else {
            pver = release;
		}
	}

	else if( !strcmp(sysname, "HP-UX") ) {
        sprintf( tmp, "HPUX" );
		if( !strcmp(release, "B.10.20") ) {
			pver = "10";
		}
		else if( !strcmp(release, "B.11.00") ) {
			pver = "11";
		}
		else if( !strcmp(release, "B.11.11") ) {
			pver = "11";
		}
		else {
			pver = release;
		}
	}
	else if ( !strncmp(sysname, "Darwin", 6) ) {
		sprintf( tmp, "OSX");
	}
	else if ( !strncmp(sysname, "AIX", 3) ) {
        sprintf(tmp, "%s", sysname);
		if ( !strcmp(version, "5") ) {
			sprintf(ver, "%s%s", version, release);
            pver = ver;
		}
	}
	else if ( !strncmp(sysname, "FreeBSD", 7) ) {
		sprintf( tmp, "FREEBSD" );
        sprintf( ver, "%c", release[0]);
        pver = ver;
	}
	else {
		sprintf( tmp, "%s", sysname);
        pver = release;
	}
    if (append_version && pver) {
        strcat( tmp, pver );
    }

	tmpopsys = strdup( tmp );
	if( !tmpopsys ) {
		EXCEPT( "Out of memory!" );
	}
	return( tmpopsys );
}
