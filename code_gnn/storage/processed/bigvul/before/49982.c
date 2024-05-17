ZEND_API void zend_objects_store_init(zend_objects_store *objects, zend_uint init_size)
{
	objects->object_buckets = (zend_object_store_bucket *) emalloc(init_size * sizeof(zend_object_store_bucket));
	objects->top = 1;  
	objects->size = init_size;
	objects->free_list_head = -1;
	memset(&objects->object_buckets[0], 0, sizeof(zend_object_store_bucket));
}
