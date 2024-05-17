void FS_GetModDescription( const char *modDir, char *description, int descriptionLen ) {
	fileHandle_t	descHandle;
	char			descPath[MAX_QPATH];
	int				nDescLen;
	FILE			*file;

	Com_sprintf( descPath, sizeof ( descPath ), "%s/description.txt", modDir );
	nDescLen = FS_SV_FOpenFileRead( descPath, &descHandle );

	if ( nDescLen > 0 && descHandle ) {
		file = FS_FileForHandle(descHandle);
		Com_Memset( description, 0, descriptionLen );
		nDescLen = fread(description, 1, descriptionLen, file);
		if (nDescLen >= 0) {
			description[nDescLen] = '\0';
		}
		FS_FCloseFile(descHandle);
	} else {
		Q_strncpyz( description, modDir, descriptionLen );
	}
}
