char *CopyString( const char *in ) {
	char    *out;

	if ( !in[0] ) {
		return ( (char *)&emptystring ) + sizeof( memblock_t );
	} else if ( !in[1] )     {
		if ( in[0] >= '0' && in[0] <= '9' ) {
			return ( (char *)&numberstring[in[0] - '0'] ) + sizeof( memblock_t );
		}
	}
	out = S_Malloc( strlen( in ) + 1 );
	strcpy( out, in );
	return out;
}
