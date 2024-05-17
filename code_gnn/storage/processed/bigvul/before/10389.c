FcCacheFindByAddrUnlocked (void *object)
{
    int	    i;
    FcCacheSkip    **next = fcCacheChains;
    FcCacheSkip    *s;

    if (!object)
	return NULL;

     
    for (i = fcCacheMaxLevel; --i >= 0;)
	while (next[i] && (char *) object >= ((char *) next[i]->cache + next[i]->size))
	    next = next[i]->next;
     
    s = next[0];
    if (s && (char *) object < ((char *) s->cache + s->size))
	return s;
    return NULL;
}
