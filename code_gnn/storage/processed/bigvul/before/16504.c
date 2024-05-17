macro_expand( const char *str )
{
	return( expand_macro(str, ConfigTab, TABLESIZE) );
}
