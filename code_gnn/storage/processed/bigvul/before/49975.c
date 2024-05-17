ZEND_API void zend_objects_store_add_ref_by_handle(zend_object_handle handle TSRMLS_DC)
{
	EG(objects_store).object_buckets[handle].bucket.obj.refcount++;
}
