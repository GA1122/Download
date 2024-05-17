FcCacheRemoveUnlocked (FcCache *cache)
{
    FcCacheSkip	    **update[FC_CACHE_MAX_LEVEL];
    FcCacheSkip	    *s, **next;
    int		    i;

     
    next = fcCacheChains;
    for (i = fcCacheMaxLevel; --i >= 0; )
    {
	for (; (s = next[i]); next = s->next)
	    if (s->cache >= cache)
		break;
        update[i] = &next[i];
    }
    s = next[0];
    for (i = 0; i < fcCacheMaxLevel && *update[i] == s; i++)
	*update[i] = s->next[i];
    while (fcCacheMaxLevel > 0 && fcCacheChains[fcCacheMaxLevel - 1] == NULL)
	fcCacheMaxLevel--;
    free (s);
}
