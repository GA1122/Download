static XRecordInterceptData *alloc_inter_data(XExtDisplayInfo *info)
{
    struct mem_cache_str *cache = (struct mem_cache_str *)info->data;
    struct intercept_queue *iq;

     
    if (cache->inter_data) {
	iq = cache->inter_data;
	cache->inter_data = iq->next;
	return &iq->data;
    }
     
    iq = (struct intercept_queue *)Xmalloc(sizeof(struct intercept_queue));
    if (!iq)
	return NULL;
    iq->cache = cache;
    cache->inter_data_count++;
    return &iq->data;
}
