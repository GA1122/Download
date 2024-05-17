FcCacheFindByStat (struct stat *cache_stat)
{
    FcCacheSkip	    *s;

    lock_cache ();
    for (s = fcCacheChains[0]; s; s = s->next[0])
	if (s->cache_dev == cache_stat->st_dev &&
	    s->cache_ino == cache_stat->st_ino &&
	    s->cache_mtime == cache_stat->st_mtime)
	{
#ifdef HAVE_STRUCT_STAT_ST_MTIM
	    if (s->cache_mtime != cache_stat->st_mtim.tv_nsec)
		continue;
#endif
	    FcRefInc (&s->ref);
	    unlock_cache ();
	    return s->cache;
	}
    unlock_cache ();
    return NULL;
}
