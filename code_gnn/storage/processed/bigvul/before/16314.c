static bool write_access(const char * filename ) {
	return thisRemoteResource->allowRemoteWriteFileAccess( filename );
}
