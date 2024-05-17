static inline void free_rmap_item(struct rmap_item *rmap_item)
{
	ksm_rmap_items--;
	rmap_item->mm = NULL;	 
	kmem_cache_free(rmap_item_cache, rmap_item);
}
