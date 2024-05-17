int	FS_FileIsInPAK(const char *filename, int *pChecksum ) {
	searchpath_t	*search;
	pack_t			*pak;
	fileInPack_t	*pakFile;
	long			hash = 0;

	if ( !fs_searchpaths ) {
		Com_Error( ERR_FATAL, "Filesystem call made without initialization" );
	}

	if ( !filename ) {
		Com_Error( ERR_FATAL, "FS_FOpenFileRead: NULL 'filename' parameter passed" );
	}

	if ( filename[0] == '/' || filename[0] == '\\' ) {
		filename++;
	}

	if ( strstr( filename, ".." ) || strstr( filename, "::" ) ) {
		return -1;
	}


	for ( search = fs_searchpaths ; search ; search = search->next ) {
		if (search->pack) {
			hash = FS_HashFileName(filename, search->pack->hashSize);
		}
		if ( search->pack && search->pack->hashTable[hash] ) {
			if ( !FS_PakIsPure(search->pack) ) {
				continue;
			}

			pak = search->pack;
			pakFile = pak->hashTable[hash];
			do {
				if ( !FS_FilenameCompare( pakFile->name, filename ) ) {
					if (pChecksum) {
						*pChecksum = pak->pure_checksum;
					}
					return 1;
				}
				pakFile = pakFile->next;
			} while(pakFile != NULL);
		}
	}
	return -1;
}
