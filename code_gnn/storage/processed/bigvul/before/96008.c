void FS_AddGameDirectory( const char *path, const char *dir ) {
	searchpath_t	*sp;
	searchpath_t	*search;
	pack_t			*pak;
	char			curpath[MAX_OSPATH + 1], *pakfile;
	int				numfiles;
	char			**pakfiles;
	int				pakfilesi;
	char			**pakfilestmp;
	int				numdirs;
	char			**pakdirs;
	int				pakdirsi;
	char			**pakdirstmp;

	int				pakwhich;
	int				len;

	for ( sp = fs_searchpaths ; sp ; sp = sp->next ) {
		if ( sp->dir && !Q_stricmp(sp->dir->path, path) && !Q_stricmp(sp->dir->gamedir, dir)) {
			return;			 
		}
	}

	Q_strncpyz( fs_gamedir, dir, sizeof( fs_gamedir ) );

	Q_strncpyz(curpath, FS_BuildOSPath(path, dir, ""), sizeof(curpath));
	curpath[strlen(curpath) - 1] = '\0';	 

	pakfiles = Sys_ListFiles(curpath, ".pk3", NULL, &numfiles, qfalse);

	qsort( pakfiles, numfiles, sizeof(char*), paksort );

	if ( fs_numServerPaks ) {
		numdirs = 0;
		pakdirs = NULL;
	} else {
		pakdirs = Sys_ListFiles(curpath, "/", NULL, &numdirs, qfalse);

		qsort( pakdirs, numdirs, sizeof(char *), paksort );
	}

	pakfilesi = 0;
	pakdirsi = 0;

	while((pakfilesi < numfiles) || (pakdirsi < numdirs))
	{
		if (pakfilesi >= numfiles) {
			pakwhich = 0;
		}
		else if (pakdirsi >= numdirs) {
			pakwhich = 1;
		}
		else {
			pakfilestmp = &pakfiles[pakfilesi];
			pakdirstmp = &pakdirs[pakdirsi];
			pakwhich = (paksort(pakfilestmp, pakdirstmp) < 0);
		}

		if (pakwhich) {
			pakfile = FS_BuildOSPath(path, dir, pakfiles[pakfilesi]);
			if ((pak = FS_LoadZipFile(pakfile, pakfiles[pakfilesi])) == 0) {
				pakfilesi++;
				continue;
			}

			Q_strncpyz(pak->pakPathname, curpath, sizeof(pak->pakPathname));
			Q_strncpyz(pak->pakGamename, dir, sizeof(pak->pakGamename));

			fs_packFiles += pak->numfiles;

			search = Z_Malloc(sizeof(searchpath_t));
			search->pack = pak;
			search->next = fs_searchpaths;
			fs_searchpaths = search;

			pakfilesi++;
		}
		else {
			len = strlen(pakdirs[pakdirsi]);
			if (!FS_IsExt(pakdirs[pakdirsi], ".pk3dir", len)) {
				pakdirsi++;
				continue;
			}

			pakfile = FS_BuildOSPath(path, dir, pakdirs[pakdirsi]);

			search = Z_Malloc(sizeof(searchpath_t));
			search->dir = Z_Malloc(sizeof(*search->dir));

			Q_strncpyz(search->dir->path, curpath, sizeof(search->dir->path));	 
			Q_strncpyz(search->dir->fullpath, pakfile, sizeof(search->dir->fullpath));	 
			Q_strncpyz(search->dir->gamedir, pakdirs[pakdirsi], sizeof(search->dir->gamedir));  

			search->next = fs_searchpaths;
			fs_searchpaths = search;

			pakdirsi++;
		}
	}

	Sys_FreeFileList( pakfiles );
	Sys_FreeFileList( pakdirs );

	search = Z_Malloc (sizeof(searchpath_t));
	search->dir = Z_Malloc( sizeof( *search->dir ) );

	Q_strncpyz(search->dir->path, path, sizeof(search->dir->path));
	Q_strncpyz(search->dir->fullpath, curpath, sizeof(search->dir->fullpath));
	Q_strncpyz(search->dir->gamedir, dir, sizeof(search->dir->gamedir));

	search->next = fs_searchpaths;
	fs_searchpaths = search;
}
