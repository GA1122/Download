qboolean FS_CL_ExtractFromPakFile( void *searchpath, const char *fullpath, const char *filename, const char *cvar_lastVersion ) {
	int srcLength;
	int destLength;
	unsigned char   *srcData;
	unsigned char   *destData;
	qboolean needToCopy;
	FILE            *destHandle;
	int	read;

	needToCopy = qtrue;

	srcLength = FS_ReadFileDir(filename, searchpath, qfalse, (void **)&srcData);

	if ( srcLength == -1 ) {
		return qfalse;
	}

	destHandle = Sys_FOpen( fullpath, "rb" );

	if ( destHandle ) {
		fseek( destHandle, 0, SEEK_END );
		destLength = ftell( destHandle );
		fseek( destHandle, 0, SEEK_SET );

		if ( destLength > 0 ) {
			destData = (unsigned char*)Z_Malloc( destLength );

			read = fread( destData, 1, destLength, destHandle );

			if (read == 0) {
				Com_Error (ERR_FATAL, "FS_CL_ExtractFromPakFile: 0 bytes read");
			}

			if ( destLength == srcLength ) {
				int i;

				for ( i = 0; i < destLength; i++ ) {
					if ( destData[i] != srcData[i] ) {
						break;
					}
				}

				if ( i == destLength ) {
					needToCopy = qfalse;
				}
			}

			Z_Free( destData );  
		}

		fclose( destHandle );
	}

	if ( needToCopy ) {
		fileHandle_t f;

		Com_DPrintf("FS_ExtractFromPakFile: FS_FOpenFileWrite '%s'\n", filename);
		f = FS_FOpenFileWrite( filename );
		if ( !f ) {
			Com_Printf( "Failed to open %s\n", filename );
			return qfalse;
		}

		FS_Write( srcData, srcLength, f );

		FS_FCloseFile( f );

#ifdef __linux__
		if ( cvar_lastVersion ) {
			Cvar_Set( cvar_lastVersion, Cvar_VariableString( "version" ) );
		}
#endif
	}

	FS_FreeFile( srcData );
	return qtrue;
}
