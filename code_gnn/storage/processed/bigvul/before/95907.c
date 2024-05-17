int     FS_FOpenFileByMode( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	int r;
	qboolean sync;

	sync = qfalse;

	switch ( mode ) {
		case FS_READ:
			r = FS_FOpenFileRead( qpath, f, qtrue );
			break;
		case FS_WRITE:
			*f = FS_FOpenFileWrite( qpath );
			r = 0;
			if (*f == 0) {
				r = -1;
			}
			break;
		case FS_APPEND_SYNC:
			sync = qtrue;
		case FS_APPEND:
			*f = FS_FOpenFileAppend( qpath );
			r = 0;
			if (*f == 0) {
				r = -1;
			}
			break;
		default:
			Com_Error( ERR_FATAL, "FS_FOpenFileByMode: bad mode" );
			return -1;
	}

	if ( !f ) {
		return r;
	}

	if ( *f ) {
		fsh[*f].fileSize = r;
		fsh[*f].streamed = qfalse;

		if (mode == FS_READ) {
			fsh[*f].streamed = qtrue;
		}
	}
	fsh[*f].handleSync = sync;

	return r;
}
