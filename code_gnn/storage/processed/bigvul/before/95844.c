qboolean FS_idPak(char *pak, char *base, int numPaks) {
	int i;

	if ( !FS_FilenameCompare( pak, va( "%s/mp_bin", base ) ) ) {
		return qtrue;
	}

	for ( i = 0; i < NUM_ID_PAKS; i++ ) {
		if ( !FS_FilenameCompare( pak, va( "%s/mp_bin%d", base, i ) ) ) {
			break;
		}
		if ( !FS_FilenameCompare( pak, va( "%s/pak%d", base, i ) ) ) {
			break;
		}
		if ( !FS_FilenameCompare( pak, va( "%s/mp_pak%d",base,i ) ) ) {
			break;
		}
		if ( !FS_FilenameCompare( pak, va( "%s/sp_pak%d",base,i + 1) ) ) {
			break;
		}
	}
	if ( i < numPaks ) {
		return qtrue;
	}
	return qfalse;
}
