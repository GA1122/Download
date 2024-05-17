ZEND_API void ZEND_FASTCALL _efree_huge(void *ptr, size_t size)
{

	ZEND_MM_CUSTOM_DEALLOCATOR(ptr);
	zend_mm_free_huge(AG(mm_heap), ptr);
}
