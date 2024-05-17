handleAll()
{
	char constraint[128];
	sprintf( constraint, "%s >= 0", ATTR_CLUSTER_ID );

	CondorError errstack;
	if( doWorkByConstraint(constraint, &errstack) ) {
		fprintf( stdout, "All jobs %s.\n",
				 (mode == JA_REMOVE_JOBS) ?
				 "marked for removal" :
				 (mode == JA_REMOVE_X_JOBS) ?
				 "removed locally (remote state unknown)" :
				 actionWord(mode,true) );
	} else {
		fprintf( stderr, "%s\n", errstack.getFullText(true) );
		if (had_error)
		{
			fprintf( stderr, "Could not %s all jobs.\n",
				 actionWord(mode,false) );
		}
	}
}
