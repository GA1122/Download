FcCacheTimeValid (FcConfig *config, FcCache *cache, struct stat *dir_stat)
{
    struct stat	dir_static;
    FcBool fnano = FcTrue;

    if (!dir_stat)
    {
	const FcChar8 *sysroot = FcConfigGetSysRoot (config);
	FcChar8 *d;

	if (sysroot)
	    d = FcStrBuildFilename (sysroot, FcCacheDir (cache), NULL);
	else
	    d = FcStrdup (FcCacheDir (cache));
	if (FcStatChecksum (d, &dir_static) < 0)
	{
	    FcStrFree (d);
	    return FcFalse;
	}
	FcStrFree (d);
	dir_stat = &dir_static;
    }
#ifdef HAVE_STRUCT_STAT_ST_MTIM
    fnano = (cache->checksum_nano == dir_stat->st_mtim.tv_nsec);
    if (FcDebug () & FC_DBG_CACHE)
	printf ("FcCacheTimeValid dir \"%s\" cache checksum %d.%ld dir checksum %d.%ld\n",
		FcCacheDir (cache), cache->checksum, (long)cache->checksum_nano, (int) dir_stat->st_mtime, dir_stat->st_mtim.tv_nsec);
#else
    if (FcDebug () & FC_DBG_CACHE)
	printf ("FcCacheTimeValid dir \"%s\" cache checksum %d dir checksum %d\n",
		FcCacheDir (cache), cache->checksum, (int) dir_stat->st_mtime);
#endif

    return cache->checksum == (int) dir_stat->st_mtime && fnano;
     return cache->checksum == (int) dir_stat->st_mtime && fnano;
 }
