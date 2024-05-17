struct cache_entry *cache_get(struct cache *cache, long long block, int size)
{
	 
	int hash = CALCULATE_HASH(block);
	struct cache_entry *entry;

	pthread_mutex_lock(&cache->mutex);

	for(entry = cache->hash_table[hash]; entry; entry = entry->hash_next)
		if(entry->block == block)
			break;

	if(entry) {
		 
		if(entry->used == 0) {
			cache->used ++;
			remove_free_list(cache, entry);
		}
		entry->used ++;
		pthread_mutex_unlock(&cache->mutex);
	} else {
		 
		if(cache->count < cache->max_buffers) {
			entry = malloc(sizeof(struct cache_entry));
			if(entry == NULL)
				EXIT_UNSQUASH("Out of memory in cache_get\n");
			entry->data = malloc(cache->buffer_size);
			if(entry->data == NULL)
				EXIT_UNSQUASH("Out of memory in cache_get\n");
			entry->cache = cache;
			entry->free_prev = entry->free_next = NULL;
			cache->count ++;
		} else {
			 
			while(cache->free_list == NULL) {
				cache->wait_free = TRUE;
				pthread_cond_wait(&cache->wait_for_free,
					&cache->mutex);
			}
			entry = cache->free_list;
			remove_free_list(cache, entry);
			remove_hash_table(cache, entry);
		}

		 
		entry->block = block;
		entry->size = size;
		entry->used = 1;
		entry->error = FALSE;
		entry->pending = TRUE;
		insert_hash_table(cache, entry);
		cache->used ++;

		 
		pthread_mutex_unlock(&cache->mutex);
		queue_put(to_reader, entry);
	}

	return entry;
}
