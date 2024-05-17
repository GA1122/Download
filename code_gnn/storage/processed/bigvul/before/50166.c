void* ZEND_FASTCALL _zend_mm_realloc2(zend_mm_heap *heap, void *ptr, size_t size, size_t copy_size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC)
{
	return zend_mm_realloc_heap(heap, ptr, size, copy_size ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
}
