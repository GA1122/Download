ZEND_API void zend_objects_store_call_destructors(zend_objects_store *objects TSRMLS_DC)
{
	zend_uint i = 1;

	for (i = 1; i < objects->top ; i++) {
		if (objects->object_buckets[i].valid) {
			struct _store_object *obj = &objects->object_buckets[i].bucket.obj;

			if (!objects->object_buckets[i].destructor_called) {
				objects->object_buckets[i].destructor_called = 1;
				if (obj->dtor && obj->object) {
					obj->refcount++;
					obj->dtor(obj->object, i TSRMLS_CC);
					obj = &objects->object_buckets[i].bucket.obj;
					obj->refcount--;

					if (obj->refcount == 0) {
						 
						GC_REMOVE_ZOBJ_FROM_BUFFER(obj);
					}
				}
			}
		}
	}
}
