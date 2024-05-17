param_integer( const char *name, int default_value,
			   int min_value, int max_value, bool use_param_table )
{
	int result;

	param_integer( name, result, true, default_value,
				   true, min_value, max_value, NULL, NULL, use_param_table );
	return result;
}
