param_boolean( const char *name, bool default_value, bool do_log,
			   ClassAd *me, ClassAd *target,
			   bool use_param_table )
{
	if(use_param_table) {
		int tbl_default_valid;
		bool tbl_default_value = 
			param_default_boolean( name, &tbl_default_valid );

		if (tbl_default_valid) {
			default_value = tbl_default_value;
		}
	}

	bool result=false;
	char *string;
	char *endptr;
	bool valid = true;

	ASSERT( name );
	string = param( name );
	
	if (!string) {
		if (do_log) {
			dprintf( D_CONFIG, "%s is undefined, using default value of %s\n",
					 name, default_value ? "True" : "False" );
		}
		return default_value;
	}

	endptr = string;
	if( strncasecmp(endptr,"true",4) == 0 ) {
		endptr+=4;
		result = true;
	}
	else if( strncasecmp(endptr,"1",1) == 0 ) {
		endptr+=1;
		result = true;
	}
	else if( strncasecmp(endptr,"false",5) == 0 ) {
		endptr+=5;
		result = false;
	}
	else if( strncasecmp(endptr,"0",1) == 0 ) {
		endptr+=1;
		result = false;
	}
	else {
		valid = false;
	}

	while( isspace(*endptr) ) {
		endptr++;
	}
	if( *endptr != '\0' ) {
		valid = false;
	}

	if( !valid ) {
		int int_value = default_value;
		ClassAd rhs;
		if( me ) {
			rhs = *me;
		}

		if( rhs.AssignExpr( name, string ) &&
			rhs.EvalBool(name,target,int_value) )
		{
			result = (int_value != 0);
			valid = true;
		}
	}

	if( !valid ) {
		EXCEPT( "%s in the condor configuration  is not a valid boolean (\"%s\")."
				"  Please set it to True or False (default is %s)",
				name, string, default_value ? "True" : "False" );
	}

	free( string );
	
	return result;
}
