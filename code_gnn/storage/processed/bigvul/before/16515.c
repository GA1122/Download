param_integer( const char *name, int &value,
			   bool use_default, int default_value,
			   bool check_ranges, int min_value, int max_value,
			   ClassAd *me, ClassAd *target,
			   bool use_param_table )
{
	if(use_param_table) {
		int tbl_default_valid;
		int tbl_default_value = 
			param_default_integer( name, &tbl_default_valid );
		bool tbl_check_ranges = 
			(param_range_integer(name, &min_value, &max_value)==-1) 
				? false : true;

		if (tbl_default_valid) {
			use_default = true;
			default_value = tbl_default_value;
		}
		if (tbl_check_ranges) {
			check_ranges = true;
		}
	}
	
	int result;
	long long_result;
	char *string;
	char *endptr = NULL;

	ASSERT( name );
	string = param( name );
	if( ! string ) {
		dprintf( D_CONFIG, "%s is undefined, using default value of %d\n",
				 name, default_value );
		if ( use_default ) {
			value = default_value;
		}
		return false;
	}

	long_result = strtol(string,&endptr,10);
	result = long_result;

	ASSERT(endptr);
	if( endptr != string ) {
		while( isspace(*endptr) ) {
			endptr++;
		}
	}
	bool valid = (endptr != string && *endptr == '\0');

	if( !valid ) {
		ClassAd rhs;
		if( me ) {
			rhs = *me;
		}
		if( !rhs.AssignExpr( name, string ) ) {
			EXCEPT("Invalid expression for %s (%s) "
				   "in condor configuration.  Please set it to "
				   "an integer expression in the range %d to %d "
				   "(default %d).",
				   name,string,min_value,max_value,default_value);
		}

		if( !rhs.EvalInteger(name,target,result) ) {
			EXCEPT("Invalid result (not an integer) for %s (%s) "
				   "in condor configuration.  Please set it to "
				   "an integer expression in the range %d to %d "
				   "(default %d).",
				   name,string,min_value,max_value,default_value);
		}
		long_result = result;
	}

	if( (long)result != long_result ) {
		EXCEPT( "%s in the condor configuration is out of bounds for"
				" an integer (%s)."
				"  Please set it to an integer in the range %d to %d"
				" (default %d).",
				name, string, min_value, max_value, default_value );
	}
	else if ( check_ranges  &&  ( result < min_value )  ) {
		EXCEPT( "%s in the condor configuration is too low (%s)."
				"  Please set it to an integer in the range %d to %d"
				" (default %d).",
				name, string, min_value, max_value, default_value );
	}
	else if ( check_ranges  && ( result > max_value )  ) {
		EXCEPT( "%s in the condor configuration is too high (%s)."
				"  Please set it to an integer in the range %d to %d"
				" (default %d).",
				name, string, min_value, max_value, default_value );
	}
	free( string );

	value = result;
	return true;
}
