static void zend_mm_add_huge_block(zend_mm_heap *heap, void *ptr, size_t size, size_t dbg_size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC)
#else
static void zend_mm_add_huge_block(zend_mm_heap *heap, void *ptr, size_t size ZEND_FILE_LINE_DC ZEND_FILE_LINE_ORIG_DC)
#endif
{
	zend_mm_huge_list *list = (zend_mm_huge_list*)zend_mm_alloc_heap(heap, sizeof(zend_mm_huge_list) ZEND_FILE_LINE_RELAY_CC ZEND_FILE_LINE_ORIG_RELAY_CC);
	list->ptr = ptr;
	list->size = size;
	list->next = heap->huge_list;
#if ZEND_DEBUG
	list->dbg.size = dbg_size;
	list->dbg.filename = __zend_filename;
	list->dbg.orig_filename = __zend_orig_filename;
	list->dbg.lineno = __zend_lineno;
	list->dbg.orig_lineno = __zend_orig_lineno;
#endif
	heap->huge_list = list;
}
