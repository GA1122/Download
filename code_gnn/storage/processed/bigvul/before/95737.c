static void CL_UpdateGUID( const char *prefix, int prefix_len )
{
#if !defined( USE_PBMD5 )
	fileHandle_t f;
	int len;

	len = FS_SV_FOpenFileRead( QKEY_FILE, &f );
	FS_FCloseFile( f );

	if( len != QKEY_SIZE ) 
		Cvar_Set( "cl_guid", "" );
	else
		Cvar_Set( "cl_guid", Com_MD5File( QKEY_FILE, QKEY_SIZE,
			prefix, prefix_len ) );
#else
	if ( !Q_stricmp( cl_cdkey, "                " )  )
	{
		Cvar_Set( "cl_guid", "NO_GUID" );

		return;
	}

	if ( !Q_stricmp( cl_guid->string, "unknown" ) )
		Cvar_Set( "cl_guid", Com_PBMD5File( cl_cdkey ) );
	else
		return;
#endif
}
