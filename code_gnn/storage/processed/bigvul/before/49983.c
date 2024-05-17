ZEND_API void zend_objects_store_mark_destructed(zend_objects_store *objects TSRMLS_DC)
{
	zend_uint i;

	if (!objects->object_buckets) {
		return;
	}
	for (i = 1; i < objects->top ; i++) {
		if (objects->object_buckets[i].valid) {
			objects->object_buckets[i].destructor_called = 1;
		}
	}
}
