void *nw_cache_alloc(nw_cache *cache)
{
    if (cache->free)
        return cache->free_arr[--cache->free];
    return malloc(cache->size);
}
