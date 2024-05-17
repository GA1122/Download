void FS_CopyFileOS( char *from, char *to ) {
	FILE    *f;
	int len;
	byte    *buf;
	char *fromOSPath, *toOSPath;

	fromOSPath = FS_BuildOSPath( fs_homepath->string, fs_gamedir, from );
	toOSPath = FS_BuildOSPath( fs_homepath->string, fs_gamedir, to );


	if ( strstr( fromOSPath, "journal.dat" ) || strstr( fromOSPath, "journaldata.dat" ) ) {
		Com_Printf( "Ignoring journal files\n" );
		return;
	}

	f = Sys_FOpen( fromOSPath, "rb" );
	if ( !f ) {
		return;
	}
	fseek( f, 0, SEEK_END );
	len = ftell( f );
	fseek( f, 0, SEEK_SET );

	buf = malloc( len );
	if ( fread( buf, 1, len, f ) != len ) {
		Com_Error( ERR_FATAL, "Short read in FS_Copyfiles()\n" );
	}
	fclose( f );

	if ( FS_CreatePath( toOSPath ) ) {
		free( buf );
		return;
	}

	f = Sys_FOpen( toOSPath, "wb" );
	if ( !f ) {
		free( buf );
		return;
	}
	if ( fwrite( buf, 1, len, f ) != len ) {
		Com_Error( ERR_FATAL, "Short write in FS_Copyfiles()\n" );
	}
	fclose( f );
	free( buf );
}
