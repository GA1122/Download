FileTransfer::DeterminePluginMethods( CondorError &e, const char* path )
{
    FILE* fp;
    const char *args[] = { path, "-classad", NULL};
    char buf[1024];

    fp = my_popenv( args, "r", FALSE );

    if( ! fp ) {
        dprintf( D_ALWAYS, "FILETRANSFER: Failed to execute %s, ignoring\n", path );
		e.pushf("FILETRANSFER", 1, "Failed to execute %s, ignoring", path );
        return "";
    }
    ClassAd* ad = new ClassAd;
    bool read_something = false;
    while( fgets(buf, 1024, fp) ) {
        read_something = true;
        if( ! ad->Insert(buf) ) {
            dprintf( D_ALWAYS, "FILETRANSFER: Failed to insert \"%s\" into ClassAd, "
                     "ignoring invalid plugin\n", buf );
            delete( ad );
            pclose( fp );
			e.pushf("FILETRANSFER", 1, "Received invalid input '%s', ignoring", buf );
            return "";
        }
    }
    my_pclose( fp );
    if( ! read_something ) {
        dprintf( D_ALWAYS,
                 "FILETRANSFER: \"%s -classad\" did not produce any output, ignoring\n",
                 path );
        delete( ad );
		e.pushf("FILETRANSFER", 1, "\"%s -classad\" did not produce any output, ignoring", path );
        return "";
    }


	char* methods = NULL;
	if (ad->LookupString( "SupportedMethods", &methods)) {
		MyString m = methods;
		free(methods);
        delete( ad );
		return m;
	}

	dprintf(D_ALWAYS, "FILETRANSFER output of \"%s -classad\" does not contain SupportedMethods, ignoring plugin\n", path );
	e.pushf("FILETRANSFER", 1, "\"%s -classad\" does not support any methods, ignoring", path );

	delete( ad );
	return "";
}
