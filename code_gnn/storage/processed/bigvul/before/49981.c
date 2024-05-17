ZEND_API zend_uint zend_objects_store_get_refcount(zval *object TSRMLS_DC)
{
	zend_object_handle handle = Z_OBJ_HANDLE_P(object);

	return EG(objects_store).object_buckets[handle].bucket.obj.refcount;
}
