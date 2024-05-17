int FS_FindVM(void **startSearch, char *found, int foundlen, const char *name, qboolean unpure, int enableQvm)
{
	searchpath_t *search, *lastSearch;
	directory_t *dir;
	pack_t *pack;
	char dllName[MAX_OSPATH], qvmName[MAX_OSPATH];
	char *netpath;

	if(!fs_searchpaths)
		Com_Error(ERR_FATAL, "Filesystem call made without initialization");

	if(enableQvm)
		Com_sprintf(qvmName, sizeof(qvmName), "vm/%s.mp.qvm", name);

	Q_strncpyz(dllName, Sys_GetDLLName(name), sizeof(dllName));

	lastSearch = *startSearch;
	if(*startSearch == NULL)
		search = fs_searchpaths;
	else
		search = lastSearch->next;
        
	while(search)
	{
		if(search->dir && (unpure || !Q_stricmp(name, "qagame")))
		{
			dir = search->dir;

			netpath = FS_BuildOSPath(dir->path, dir->gamedir, dllName);

			if(enableQvm && FS_FOpenFileReadDir(qvmName, search, NULL, qfalse, unpure) > 0)
			{
				*startSearch = search;
				return VMI_COMPILED;
			}

			if(dir->allowUnzippedDLLs && FS_FileInPathExists(netpath))
			{
				Q_strncpyz(found, netpath, foundlen);
				*startSearch = search;
				
				return VMI_NATIVE;
			}
		}
		else if(search->pack)
		{
			pack = search->pack;

		        if(lastSearch && lastSearch->pack)
		        {
		                
		                if(!FS_FilenameCompare(lastSearch->pack->pakPathname, pack->pakPathname))
                                {
                                        search = search->next;
                                        continue;
                                }
		        }

			if(enableQvm && FS_FOpenFileReadDir(qvmName, search, NULL, qfalse, unpure) > 0)
			{
				*startSearch = search;

				return VMI_COMPILED;
			}

#ifndef DEDICATED
			if (Q_stricmp(name, "qagame"))
			{
				netpath = FS_BuildOSPath(fs_homepath->string, pack->pakGamename, dllName);

				if (FS_FOpenFileReadDir(dllName, search, NULL, qfalse, unpure) > 0
						&& FS_CL_ExtractFromPakFile(search, netpath, dllName, NULL))
				{
					Com_Printf( "Loading %s dll from %s\n", name, pack->pakFilename );
					Q_strncpyz(found, netpath, foundlen);
					*startSearch = search;

					return VMI_NATIVE;
				}
			}
#endif
		}
		
		search = search->next;
	}

	return -1;
}
