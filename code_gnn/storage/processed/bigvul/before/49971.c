ZEND_API void zend_object_store_set_object(zval *zobject, void *object TSRMLS_DC)
{
	zend_object_handle handle = Z_OBJ_HANDLE_P(zobject);

	EG(objects_store).object_buckets[handle].bucket.obj.object = object;
}
