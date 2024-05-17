void FS_AddGameDirectory( const char *path, const char *dir, qboolean allowUnzippedDLLs ) {
	searchpath_t    *sp;
	int i;
	searchpath_t    *search;
	pack_t          *pak;
	char            curpath[MAX_OSPATH + 1], *pakfile;
	int numfiles;
	char            **pakfiles;
	char            *sorted[MAX_PAKFILES];

	Q_strncpyz(curpath, FS_BuildOSPath(path, dir, ""), sizeof(curpath));
	curpath[strlen(curpath) - 1] = '\0';	 

	for ( sp = fs_searchpaths ; sp ; sp = sp->next ) {
		if ( sp->dir && !Q_stricmp( sp->dir->path, path ) && !Q_stricmp( sp->dir->gamedir, dir ) ) {
			return;          
		}
	}

	Q_strncpyz( fs_gamedir, dir, sizeof( fs_gamedir ) );

	pakfile = FS_BuildOSPath( path, dir, "" );
	pakfile[ strlen( pakfile ) - 1 ] = 0;  

	pakfiles = Sys_ListFiles( pakfile, ".pk3", NULL, &numfiles, qfalse );

	if ( numfiles > MAX_PAKFILES ) {
		numfiles = MAX_PAKFILES;
	}
	for ( i = 0 ; i < numfiles ; i++ ) {
		sorted[i] = pakfiles[i];

		if ( !Q_strncmp( sorted[i],"mp_",3 ) ) {
			memcpy( sorted[i],"zz",2 );
		}

	}

	qsort( sorted, numfiles, sizeof(char*), paksort );

	for ( i = 0 ; i < numfiles ; i++ ) {
		if ( Q_strncmp( sorted[i],"sp_",3 ) ) {  

			if ( !Q_strncmp( sorted[i],"zz_",3 ) ) {
				memcpy( sorted[i],"mp",2 );
			}

			pakfile = FS_BuildOSPath( path, dir, sorted[i] );
			if ( ( pak = FS_LoadZipFile( pakfile, sorted[i] ) ) == 0 ) {
				continue;
			}
			Q_strncpyz(pak->pakPathname, curpath, sizeof(pak->pakPathname));
			strcpy( pak->pakGamename, dir );

			search = Z_Malloc( sizeof( searchpath_t ) );
			search->pack = pak;
			search->next = fs_searchpaths;
			fs_searchpaths = search;
		}
	}

	Sys_FreeFileList( pakfiles );

	search = Z_Malloc (sizeof(searchpath_t));
	search->dir = Z_Malloc( sizeof( *search->dir ) );

	Q_strncpyz(search->dir->path, path, sizeof(search->dir->path));
	Q_strncpyz(search->dir->fullpath, curpath, sizeof(search->dir->fullpath));
	Q_strncpyz(search->dir->gamedir, dir, sizeof(search->dir->gamedir));
	search->dir->allowUnzippedDLLs = allowUnzippedDLLs;

	search->next = fs_searchpaths;
	fs_searchpaths = search;
}