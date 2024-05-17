handleConstraints( void )
{
	if( ! has_constraint ) {
		return;
	}
	const char* tmp = global_constraint.Value();

	CondorError errstack;
	if( doWorkByConstraint(tmp, &errstack) ) {
		fprintf( stdout, "Jobs matching constraint %s %s\n", tmp,
				 (mode == JA_REMOVE_JOBS) ?
				 "have been marked for removal" :
				 (mode == JA_REMOVE_X_JOBS) ?
				 "have been removed locally (remote state unknown)" :
				 actionWord(mode,true) );

	} else {
		fprintf( stderr, "%s\n", errstack.getFullText(true) );
		if (had_error)
		{
			fprintf( stderr, 
				 "Couldn't find/%s all jobs matching constraint %s\n",
				 actionWord(mode,false), tmp );
		}
	}
}
