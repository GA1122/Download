FcCacheFindByAddr (void *object)
{
    FcCacheSkip *ret;
    lock_cache ();
    ret = FcCacheFindByAddrUnlocked (object);
    unlock_cache ();
    return ret;
}
