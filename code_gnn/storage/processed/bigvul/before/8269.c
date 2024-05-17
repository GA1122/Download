static XPointer alloc_mem_cache(void)
{
    struct mem_cache_str *cache;

     
    cache = (struct mem_cache_str *) Xmalloc(sizeof(struct mem_cache_str));
    if (cache) {
	cache->display_closed = False;
	cache->inter_data = NULL;
	cache->inter_data_count = 0;
	cache->reply_buffers = NULL;
    }
    return (XPointer) cache;
}
