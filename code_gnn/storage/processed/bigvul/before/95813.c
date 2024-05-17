int FS_Read( void *buffer, int len, fileHandle_t f ) {
	int block, remaining;
	int read;
	byte    *buf;
	int tries;

	if ( !fs_searchpaths ) {
		Com_Error( ERR_FATAL, "Filesystem call made without initialization" );
	}

	if ( !f ) {
		return 0;
	}

	buf = (byte *)buffer;
	fs_readCount += len;

	if ( fsh[f].zipFile == qfalse ) {
		remaining = len;
		tries = 0;
		while ( remaining ) {
			block = remaining;
			read = fread( buf, 1, block, fsh[f].handleFiles.file.o );
			if ( read == 0 ) {
				if ( !tries ) {
					tries = 1;
				} else {
					return len - remaining;    
				}
			}

			if ( read == -1 ) {
				Com_Error( ERR_FATAL, "FS_Read: -1 bytes read" );
			}

			remaining -= read;
			buf += read;
		}
		return len;
	} else {
		return unzReadCurrentFile( fsh[f].handleFiles.file.z, buffer, len );
	}
}