 CatalogueRescan (FontPathElementPtr fpe)
 {
     CataloguePtr	cat = fpe->private;
     char		link[MAXFONTFILENAMELEN];
     char		dest[MAXFONTFILENAMELEN];
     char		*attrib;
     FontPathElementPtr	subfpe;
     struct stat		statbuf;
     const char		*path;
     DIR			*dir;
     struct dirent	*entry;
     int			len;
     int			pathlen;
 
     path = fpe->name + strlen(CataloguePrefix);
     if (stat(path, &statbuf) < 0 || !S_ISDIR(statbuf.st_mode))
 	return BadFontPath;
 
     if (statbuf.st_mtime <= cat->mtime)
 	return Successful;
 
     dir = opendir(path);
     if (dir == NULL)
     {
 	xfree(cat);
 	return BadFontPath;
     }
 
     CatalogueUnrefFPEs (fpe);
      while (entry = readdir(dir), entry != NULL)
      {
  	snprintf(link, sizeof link, "%s/%s", path, entry->d_name);
	len = readlink(link, dest, sizeof dest);
// 	len = readlink(link, dest, sizeof dest - 1);
  	if (len < 0)
  	    continue;
 	dest[len] = '\0';
 
 	if (dest[0] != '/')
 	{
 	   pathlen = strlen(path);
 	   memmove(dest + pathlen + 1, dest, sizeof dest - pathlen - 1);
 	   memcpy(dest, path, pathlen);
 	   memcpy(dest + pathlen, "/", 1);
 	   len += pathlen + 1;
 	}
 
 	attrib = strchr(link, ':');
 	if (attrib && len + strlen(attrib) < sizeof dest)
 	{
 	    memcpy(dest + len, attrib, strlen(attrib));
 	    len += strlen(attrib);
 	}
 
 	subfpe = xalloc(sizeof *subfpe);
 	if (subfpe == NULL)
 	    continue;
 
 	 
 	subfpe->type = fpe->type;
 	subfpe->name_length = len;
 	subfpe->name = xalloc (len + 1);
 	if (subfpe == NULL)
 	{
 	    xfree(subfpe);
 	    continue;
 	}
 
 	memcpy(subfpe->name, dest, len);
 	subfpe->name[len] = '\0';
 
 	 
 	subfpe->refcount = 1;
 
 	if (FontFileInitFPE (subfpe) != Successful)
 	{
 	    xfree(subfpe->name);
 	    xfree(subfpe);
 	    continue;
 	}
 
 	if (CatalogueAddFPE(cat, subfpe) != Successful)
 	{
 	    FontFileFreeFPE (subfpe);
 	    xfree(subfpe);
 	    continue;
 	}
     }
 
     closedir(dir);
 
     qsort(cat->fpeList,
 	  cat->fpeCount, sizeof cat->fpeList[0], ComparePriority);
 
     cat->mtime = statbuf.st_mtime;
 
     return Successful;
 }