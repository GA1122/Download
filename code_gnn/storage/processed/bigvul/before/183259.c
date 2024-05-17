 long FS_FOpenFileRead(const char *filename, fileHandle_t *file, qboolean uniqueFILE)
  {
  	searchpath_t *search;
  	long len;
// 	qboolean isLocalConfig;
  
  	if(!fs_searchpaths)
  		Com_Error(ERR_FATAL, "Filesystem call made without initialization");
  
// 	isLocalConfig = !strcmp(filename, "autoexec.cfg") || !strcmp(filename, Q3CONFIG_CFG);
  	for(search = fs_searchpaths; search; search = search->next)
  	{
// 		 
// 		if (isLocalConfig && search->pack)
// 			continue;
// 
  		len = FS_FOpenFileReadDir(filename, search, file, uniqueFILE, qfalse);
  
  		if(file == NULL)
 		{
 			if(len > 0)
 				return len;
 		}
 		else
 		{
 			if(len >= 0 && *file)
 				return len;
 		}
 
 	}
 	
 #ifdef FS_MISSING
 	if(missingFiles)
 		fprintf(missingFiles, "%s\n", filename);
 #endif
 
 	if(file)
 	{
 		*file = 0;
 		return -1;
 	}
 	else
 	{
 		return 0;
 	}
 }