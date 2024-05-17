nw_cache *nw_cache_create(uint32_t size)
{
    nw_cache *cache = malloc(sizeof(nw_cache));
    if (cache == NULL)
        return NULL;

    cache->size = size;
    cache->used = 0;
    cache->free = 0;
    cache->free_total = NW_CACHE_INIT_SIZE;
    cache->free_arr = malloc(cache->free_total * sizeof(void *));
    if (cache->free_arr == NULL) {
        free(cache);
        return NULL;
    }

    return cache;
}
