FcCacheObjectReference (void *object)
{
    FcCacheSkip *skip = FcCacheFindByAddr (object);

    if (skip)
	FcRefInc (&skip->ref);
}
