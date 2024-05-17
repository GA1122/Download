void nw_cache_release(nw_cache *cache)
{
    for (uint32_t i = 0; i < cache->free; ++i) {
        free(cache->free_arr[i]);
    }
    free(cache->free_arr);
    free(cache);
}
