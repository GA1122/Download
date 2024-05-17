qboolean FS_PakIsPure( pack_t *pack ) {
	int i;

	if ( fs_numServerPaks ) {
		for ( i = 0 ; i < fs_numServerPaks ; i++ ) {
			if ( pack->checksum == fs_serverPaks[i] ) {
				return qtrue;        
			}
		}
		return qfalse;   
	}
	return qtrue;
}
