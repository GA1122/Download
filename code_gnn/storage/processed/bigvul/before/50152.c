ZEND_API void* ZEND_FASTCALL _emalloc_huge(size_t size)
{

	ZEND_MM_CUSTOM_ALLOCATOR(size);
	return zend_mm_alloc_huge(AG(mm_heap), size);
}
