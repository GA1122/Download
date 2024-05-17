int param_integer_c( const char *name, int default_value,
					   int min_value, int max_value, bool use_param_table )
{
	return param_integer( name, default_value, min_value, max_value, use_param_table );
}
