void jas_free(void *ptr)
{
	jas_mb_t *mb;
	size_t mem;
	size_t size;
	JAS_DBGLOG(100, ("jas_free(%p)\n", ptr));
	if (ptr) {
		mb = jas_get_mb(ptr);
		size = mb->size;
		JAS_DBGLOG(101, ("jas_free(%p) (mb=%p; size=%zu)\n", ptr, mb, size));
		if (!jas_safe_size_sub(jas_mem, size, &jas_mem)) {
			jas_eprintf("heap corruption detected\n");
			abort();
		}
		JAS_DBGLOG(100, ("jas_free: free(%p)\n", mb));
		free(mb);
	}
	JAS_DBGLOG(102, ("max_mem=%zu; mem=%zu\n", jas_max_mem, jas_mem));
}
