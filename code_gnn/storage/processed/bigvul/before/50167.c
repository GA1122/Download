static void alloc_globals_ctor(zend_alloc_globals *alloc_globals)
{
#if ZEND_MM_CUSTOM
	char *tmp = getenv("USE_ZEND_ALLOC");

	if (tmp && !zend_atoi(tmp, 0)) {
		alloc_globals->mm_heap = malloc(sizeof(zend_mm_heap));
		memset(alloc_globals->mm_heap, 0, sizeof(zend_mm_heap));
		alloc_globals->mm_heap->use_custom_heap = ZEND_MM_CUSTOM_HEAP_STD;
		alloc_globals->mm_heap->custom_heap.std._malloc = malloc;
		alloc_globals->mm_heap->custom_heap.std._free = free;
		alloc_globals->mm_heap->custom_heap.std._realloc = realloc;
		return;
	}
#endif
#ifdef MAP_HUGETLB
	tmp = getenv("USE_ZEND_ALLOC_HUGE_PAGES");
	if (tmp && zend_atoi(tmp, 0)) {
		zend_mm_use_huge_pages = 1;
	}
#endif
	ZEND_TSRMLS_CACHE_UPDATE();
	alloc_globals->mm_heap = zend_mm_init();
}
