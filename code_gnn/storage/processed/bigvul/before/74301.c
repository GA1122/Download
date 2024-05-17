void remove_free_list(struct cache *cache, struct cache_entry *entry)
{
	if(entry->free_prev == NULL || entry->free_next == NULL)
		 
		return;
	else if(entry->free_prev == entry && entry->free_next == entry) {
		 
		cache->free_list = NULL;
	} else {
		 
		entry->free_next->free_prev = entry->free_prev;
		entry->free_prev->free_next = entry->free_next;
		if(cache->free_list == entry)
			cache->free_list = entry->free_next;
	}

	entry->free_prev = entry->free_next = NULL;
}
