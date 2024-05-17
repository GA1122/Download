static FILE *FS_FileForHandle( fileHandle_t f ) {
	if ( f < 1 || f >= MAX_FILE_HANDLES ) {
		Com_Error( ERR_DROP, "FS_FileForHandle: out of range" );
	}
	if ( fsh[f].zipFile == qtrue ) {
		Com_Error( ERR_DROP, "FS_FileForHandle: can't get FILE on zip file" );
	}
	if ( !fsh[f].handleFiles.file.o ) {
		Com_Error( ERR_DROP, "FS_FileForHandle: NULL" );
	}

	return fsh[f].handleFiles.file.o;
}