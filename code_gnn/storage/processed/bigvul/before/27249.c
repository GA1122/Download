static inline struct mm_slot *alloc_mm_slot(void)
{
	if (!mm_slot_cache)	 
		return NULL;
	return kmem_cache_zalloc(mm_slot_cache, GFP_KERNEL);
}
