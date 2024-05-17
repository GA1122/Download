void update_cache_mode_entry(unsigned entry, enum page_cache_mode cache)
{
	 
	BUG_ON(!entry && cache != _PAGE_CACHE_MODE_WB);

	__cachemode2pte_tbl[cache] = __cm_idx2pte(entry);
	__pte2cachemode_tbl[entry] = cache;
}
