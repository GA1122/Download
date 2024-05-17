int     FS_FTell( fileHandle_t f ) {
	int pos;
	if ( fsh[f].zipFile == qtrue ) {
		pos = unztell( fsh[f].handleFiles.file.z );
	} else {
		pos = ftell( fsh[f].handleFiles.file.o );
	}
	return pos;
}
