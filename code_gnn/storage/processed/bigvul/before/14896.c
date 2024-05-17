static void spl_heap_it_get_current_data(zend_object_iterator *iter, zval ***data TSRMLS_DC)  
{
	spl_heap_it  *iterator = (spl_heap_it *)iter;
	zval        **element  = (zval **)&iterator->object->heap->elements[0];

	if (iterator->object->heap->flags & SPL_HEAP_CORRUPTED) {
		zend_throw_exception(spl_ce_RuntimeException, "Heap is corrupted, heap properties are no longer ensured.", 0 TSRMLS_CC);
		return;
	}

	if (iterator->object->heap->count == 0 || !*element) {
		*data = NULL;
	} else {
		*data = element;
	}
}
 