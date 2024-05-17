is_ckpt_file( const char *name )
{

	if( strstr(name,"cluster") ) {
		return is_v3_ckpt( name );
	} else {
		return is_v2_ckpt( name );
	}

}
