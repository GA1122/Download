procArg(const char* arg)
{
	int		c, p;								 
	char*	tmp;

	MyString constraint;

	if( str_isint(arg) || str_isreal(arg,true) )
	{
		c = strtol(arg, &tmp, 10);
		if(c <= 0)
		{
			fprintf(stderr, "Invalid cluster # from %s.\n", arg);
			had_error = true;
			return;
		}
		if(*tmp == '\0')
		{
			CondorError errstack;
			constraint.sprintf( "%s == %d", ATTR_CLUSTER_ID, c );
			if( doWorkByConstraint(constraint.Value(), &errstack) ) {
				fprintf( stdout, 
						 "Cluster %d %s.\n", c,
						 (mode == JA_REMOVE_JOBS) ?
						 "has been marked for removal" :
						 (mode == JA_REMOVE_X_JOBS) ?
						 "has been removed locally (remote state unknown)" :
						 actionWord(mode,true) );
			} else {
				fprintf( stderr, "%s\n", errstack.getFullText(true) );
				if (had_error)
				{
					fprintf( stderr, 
						 "Couldn't find/%s all jobs in cluster %d.\n",
						 actionWord(mode,false), c );
				}
			}
			return;
		}
		if(*tmp == '.')
		{
			p = strtol(tmp + 1, &tmp, 10);
			if(p < 0)
			{
				fprintf( stderr, "Invalid proc # from %s.\n", arg);
				had_error = true;
				return;
			}
			if(*tmp == '\0')
			{
				if( ! job_ids ) {
					job_ids = new StringList();
				}
				job_ids->append( arg );
				return;
			}
		}
		fprintf( stderr, "Warning: unrecognized \"%s\" skipped.\n", arg );
		return;
	}
	else {
		CondorError errstack;
		constraint.sprintf("%s == \"%s\"", ATTR_OWNER, arg );
		if( doWorkByConstraint(constraint.Value(), &errstack) ) {
			fprintf( stdout, "User %s's job(s) %s.\n", arg,
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
					 "Couldn't find/%s all of user %s's job(s).\n",
					 actionWord(mode,false), arg );
			}
		}
	}
}
