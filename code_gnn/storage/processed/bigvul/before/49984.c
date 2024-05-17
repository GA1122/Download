ZEND_API zend_object_handle zend_objects_store_put(void *object, zend_objects_store_dtor_t dtor, zend_objects_free_object_storage_t free_storage, zend_objects_store_clone_t clone TSRMLS_DC)
{
	zend_object_handle handle;
	struct _store_object *obj;

	if (EG(objects_store).free_list_head != -1) {
		handle = EG(objects_store).free_list_head;
		EG(objects_store).free_list_head = EG(objects_store).object_buckets[handle].bucket.free_list.next;
	} else {
		if (EG(objects_store).top == EG(objects_store).size) {
			EG(objects_store).size <<= 1;
			EG(objects_store).object_buckets = (zend_object_store_bucket *) erealloc(EG(objects_store).object_buckets, EG(objects_store).size * sizeof(zend_object_store_bucket));
		}
		handle = EG(objects_store).top++;
	}
	obj = &EG(objects_store).object_buckets[handle].bucket.obj;
	EG(objects_store).object_buckets[handle].destructor_called = 0;
	EG(objects_store).object_buckets[handle].valid = 1;
	EG(objects_store).object_buckets[handle].apply_count = 0;

	obj->refcount = 1;
	GC_OBJ_INIT(obj);
	obj->object = object;
	obj->dtor = dtor?dtor:(zend_objects_store_dtor_t)zend_objects_destroy_object;
	obj->free_storage = free_storage;
	obj->clone = clone;
	obj->handlers = NULL;

#if ZEND_DEBUG_OBJECTS
	fprintf(stderr, "Allocated object id #%d\n", handle);
#endif
	return handle;
}
