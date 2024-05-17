FcCacheObjectDereference (void *object)
{
    FcCacheSkip	*skip;

    lock_cache ();
    skip = FcCacheFindByAddrUnlocked (object);
    if (skip)
    {
	if (FcRefDec (&skip->ref) == 1)
	    FcDirCacheDisposeUnlocked (skip->cache);
    }
    unlock_cache ();
}
