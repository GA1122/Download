ZEND_API void zend_objects_store_free_object_storage(zend_objects_store *objects TSRMLS_DC)
{
	zend_uint i = 1;

	for (i = 1; i < objects->top ; i++) {
		if (objects->object_buckets[i].valid) {
			struct _store_object *obj = &objects->object_buckets[i].bucket.obj;

			GC_REMOVE_ZOBJ_FROM_BUFFER(obj);

			objects->object_buckets[i].valid = 0;
			if (obj->free_storage) {
				obj->free_storage(obj->object TSRMLS_CC);
			}
			 
		}
	}
}
