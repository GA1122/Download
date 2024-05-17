static inline void free_thread_info(struct thread_info *ti)
{
	free_memcg_kmem_pages((unsigned long)ti, THREAD_SIZE_ORDER);
}
