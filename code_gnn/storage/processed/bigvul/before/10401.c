FcDirCacheUnlink (const FcChar8 *dir, FcConfig *config)
{
    FcChar8	*cache_hashed = NULL;
    FcChar8	cache_base[CACHEBASE_LEN];
    FcStrList	*list;
    FcChar8	*cache_dir;
    const FcChar8 *sysroot = FcConfigGetSysRoot (config);

    FcDirCacheBasename (dir, cache_base);

    list = FcStrListCreate (config->cacheDirs);
    if (!list)
        return FcFalse;
	
    while ((cache_dir = FcStrListNext (list)))
    {
	if (sysroot)
	    cache_hashed = FcStrBuildFilename (sysroot, cache_dir, cache_base, NULL);
	else
	    cache_hashed = FcStrBuildFilename (cache_dir, cache_base, NULL);
        if (!cache_hashed)
	    break;
	(void) unlink ((char *) cache_hashed);
	FcStrFree (cache_hashed);
    }
    FcStrListDone (list);
     
    if (cache_dir)
	return FcFalse;
    return FcTrue;
}
