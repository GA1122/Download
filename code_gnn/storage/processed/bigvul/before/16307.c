printNewMessages( ClassAd* result_ad, StringList* ids )
{
	char* tmp;
	char* msg;
	PROC_ID job_id;
	bool rval;

	JobActionResults results;
	results.readResults( result_ad );

	ids->rewind();
	while( (tmp = ids->next()) ) {
		job_id = getProcByString( tmp );
		rval = results.getResultString( job_id, &msg );
		if( rval ) {
			fprintf( stdout, "%s\n", msg );
		} else {
			fprintf( stderr, "%s\n", msg );
		}
		free( msg );
	}
}
