static void FS_CheckFilenameIsMutable( const char *filename,
		const char *function )
{
	if( COM_CompareExtension( filename, DLL_EXT )
		|| COM_CompareExtension( filename, ".qvm" )
		|| COM_CompareExtension( filename, ".pk3" ) )
	{
		Com_Error( ERR_FATAL, "%s: Not allowed to manipulate '%s' due "
			"to %s extension", function, filename, COM_GetExtension( filename ) );
	}
}
