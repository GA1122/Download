ZEND_API void zend_objects_store_add_ref(zval *object TSRMLS_DC)
{
	zend_object_handle handle = Z_OBJ_HANDLE_P(object);

	EG(objects_store).object_buckets[handle].bucket.obj.refcount++;
#if ZEND_DEBUG_OBJECTS
	fprintf(stderr, "Increased refcount of object id #%d\n", handle);
#endif
}
