FcCacheFini (void)
{
    int		    i;

    for (i = 0; i < FC_CACHE_MAX_LEVEL; i++)
	assert (fcCacheChains[i] == NULL);
    assert (fcCacheMaxLevel == 0);

    free_lock ();
}
