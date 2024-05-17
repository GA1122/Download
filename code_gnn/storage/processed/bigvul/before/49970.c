ZEND_API void *zend_object_store_get_object_by_handle(zend_object_handle handle TSRMLS_DC)
{
	return EG(objects_store).object_buckets[handle].bucket.obj.object;
}
