static inline void set_store_user_dirty(struct kmem_cache *cachep)
{
	if (is_store_user_clean(cachep))
		atomic_set(&cachep->store_user_clean, 0);
}
