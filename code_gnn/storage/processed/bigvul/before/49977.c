ZEND_API zend_object_value zend_objects_store_clone_obj(zval *zobject TSRMLS_DC)
{
	zend_object_value retval;
	void *new_object;
	struct _store_object *obj;
	zend_object_handle handle = Z_OBJ_HANDLE_P(zobject);

	obj = &EG(objects_store).object_buckets[handle].bucket.obj;

	if (obj->clone == NULL) {
		zend_error(E_CORE_ERROR, "Trying to clone uncloneable object of class %s", Z_OBJCE_P(zobject)->name);
	}

	obj->clone(obj->object, &new_object TSRMLS_CC);
	obj = &EG(objects_store).object_buckets[handle].bucket.obj;

	retval.handle = zend_objects_store_put(new_object, obj->dtor, obj->free_storage, obj->clone TSRMLS_CC);
	retval.handlers = Z_OBJ_HT_P(zobject);
	EG(objects_store).object_buckets[handle].bucket.obj.handlers = retval.handlers;

	return retval;
}
