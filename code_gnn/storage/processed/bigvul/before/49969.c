ZEND_API void *zend_object_store_get_object(const zval *zobject TSRMLS_DC)
{
	zend_object_handle handle = Z_OBJ_HANDLE_P(zobject);

	return EG(objects_store).object_buckets[handle].bucket.obj.object;
}
