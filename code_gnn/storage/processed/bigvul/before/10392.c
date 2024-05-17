FcCacheInsert (FcCache *cache, struct stat *cache_stat)
{
    FcCacheSkip    **update[FC_CACHE_MAX_LEVEL];
    FcCacheSkip    *s, **next;
    int		    i, level;

    lock_cache ();

     
    next = fcCacheChains;
    for (i = fcCacheMaxLevel; --i >= 0; )
    {
	for (; (s = next[i]); next = s->next)
	    if (s->cache > cache)
		break;
        update[i] = &next[i];
    }

     
    level = random_level ();
    if (level > fcCacheMaxLevel)
    {
	level = fcCacheMaxLevel + 1;
	update[fcCacheMaxLevel] = &fcCacheChains[fcCacheMaxLevel];
	fcCacheMaxLevel = level;
    }

    s = malloc (sizeof (FcCacheSkip) + (level - 1) * sizeof (FcCacheSkip *));
    if (!s)
	return FcFalse;

    s->cache = cache;
    s->size = cache->size;
    FcRefInit (&s->ref, 1);
    if (cache_stat)
    {
	s->cache_dev = cache_stat->st_dev;
	s->cache_ino = cache_stat->st_ino;
	s->cache_mtime = cache_stat->st_mtime;
#ifdef HAVE_STRUCT_STAT_ST_MTIM
	s->cache_mtime_nano = cache_stat->st_mtim.tv_nsec;
#else
	s->cache_mtime_nano = 0;
#endif
    }
    else
    {
	s->cache_dev = 0;
	s->cache_ino = 0;
	s->cache_mtime = 0;
	s->cache_mtime_nano = 0;
    }

     
    for (i = 0; i < level; i++)
    {
	s->next[i] = *update[i];
	*update[i] = s;
    }

    unlock_cache ();
    return FcTrue;
}
