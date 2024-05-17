param_double( const char *name, double default_value,
			  double min_value, double max_value,
			  ClassAd *me, ClassAd *target,
			  bool use_param_table )
{
	if(use_param_table) {
		int tbl_default_valid;
		double tbl_default_value = 
			param_default_double( name, &tbl_default_valid );

		param_range_double(name, &min_value, &max_value);

		if (tbl_default_valid) {
			default_value = tbl_default_value;
		}
	}
	
	double result;
	char *string;
	char *endptr = NULL;

	ASSERT( name );
	string = param( name );
	
	if( ! string ) {
		dprintf( D_CONFIG, "%s is undefined, using default value of %f\n",
				 name, default_value );
		return default_value;
	}

	result = strtod(string,&endptr);

	ASSERT(endptr);
	if( endptr != string ) {
		while( isspace(*endptr) ) {
			endptr++;
		}
	}
	bool valid = (endptr != string && *endptr == '\0');
	if( !valid ) {
		ClassAd rhs;
		float float_result;
		if( me ) {
			rhs = *me;
		}
		if( !rhs.AssignExpr( name, string ) ) {
			EXCEPT("Invalid expression for %s (%s) "
				   "in condor configuration.  Please set it to "
				   "a numeric expression in the range %lg to %lg "
				   "(default %lg).",
				   name,string,min_value,max_value,default_value);
		}

		if( !rhs.EvalFloat(name,target,float_result) ) {
			EXCEPT("Invalid result (not a number) for %s (%s) "
				   "in condor configuration.  Please set it to "
				   "a numeric expression in the range %lg to %lg "
				   "(default %lg).",
				   name,string,min_value,max_value,default_value);
		}
		result = float_result;
	}

	if( result < min_value ) {
		EXCEPT( "%s in the condor configuration is too low (%s)."
				"  Please set it to a number in the range %lg to %lg"
				" (default %lg).",
				name, string, min_value, max_value, default_value );
	}
	else if( result > max_value ) {
		EXCEPT( "%s in the condor configuration is too high (%s)."
				"  Please set it to a number in the range %lg to %lg"
				" (default %lg).",
				name, string, min_value, max_value, default_value );
	}
	free( string );
	return result;
}
